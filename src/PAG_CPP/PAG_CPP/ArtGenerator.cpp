#define _USE_MATH_DEFINES

#include "ArtGenerator.h"

#include <cmath>

namespace godot
{

ArtGenerator::ArtGenerator()
	: mPixelSize(16)
	, mAssetSize(64)
	, mScreenSize(Vector2(1280, 720)) 
{
	resetPixelArray();

	Color* black = new Color(BLACK->r, BLACK->g, BLACK->b);





	std::vector<std::vector<Color*>> smileArr;
	smileArr.resize(5);
	for (int x = 0; x < smileArr.size(); x++)
		smileArr[x].resize(5);

	smileArr[1][1] = black;
	smileArr[1][3] = black;
	smileArr[3][0] = black;
	smileArr[3][4] = black;
	smileArr[4][1] = black;
	smileArr[4][2] = black;
	smileArr[4][3] = black;

	for (int x = 0; x < smileArr.size(); x++) 
	{
		for (int y = 0; y < smileArr[x].size(); y++) 
		{
			if (smileArr[x][y] != nullptr)
				setPixel(Vector2(x, y), smileArr[x][y]);
		}
	}

	pivotPixelArray();




	addLine(Vector2(10, 20), Vector2(20, 10), black);

	addCircle(Vector2(31, 31), 5.0f, 12, black);
	fillColor(Vector2(31, 31), new Color(1.0f, 1.0f, 1.0f, 1.0f));


	std::vector<Vector2> triangle;
	triangle.push_back(Vector2(40, 10));
	triangle.push_back(Vector2(40, 20));
	triangle.push_back(Vector2(50, 20));
	addShape(triangle, black, new Color(1.0f, 1.0f, 1.0f, 1.0f));

}

ArtGenerator::~ArtGenerator()
{

}

void ArtGenerator::_register_methods()
{
	register_method((char*)"_ready", &ready);
	register_method((char*)"_draw", &draw);
}

void ArtGenerator::_init()
{
	//if (0 == 1)
	//	return;
}

void ArtGenerator::ready()
{

}

void ArtGenerator::draw()
{
	drawPixelArray();
}

void ArtGenerator::drawPixel(Vector2 pos, Color* color)
{
	Rect2 rect = Rect2(pos.x, pos.y, mPixelSize, mPixelSize);
	draw_rect(rect, *color);
}

void ArtGenerator::drawPixelArray()
{
	for (int i = 0; i < mPixelArray.size(); i++) 
	{
		for (int j = 0; j < mPixelArray[i].size(); j++)
		{
			if (mPixelArray[i][j] != nullptr)
			{
				drawPixel(Vector2(i*mPixelSize, j*mPixelSize), mPixelArray[i][j]);
			}
		}
	}
}

void ArtGenerator::setPixel(Vector2 pos, Color* color)
{
	mPixelArray[pos.x][pos.y] = color;
}

void ArtGenerator::addLine(Vector2 origin, Vector2 dest, Color* color)
{
	float stepSize = 1.0f / mAssetSize;

	Vector2 dir = dest - origin;

	// If a line is more vertical than horizontal, it should not have more than 1 pixel in same y coord. aka jaggies
	bool isVertical;
	if (abs(dir.y) >= abs(dir.x))
		isVertical = true;
	else
		isVertical = false;

	Vector2 temp = origin;

	// Holds prev XY coord, depending on if line is horizontal or vertical, which is then used to remove jaggies
	int lastXY = 0;
	int thisXY = 0;

	// Coef of line, 0 is first pixel, 1 is last pixel
	float coef = 0.0f;

	while (coef <= 1.0f) 
	{
		temp = Vector2(
			round(origin.x * (1.0f - coef) + dest.x * coef), 
			round(origin.y * (1.0f - coef) + dest.y * coef)
		);

		coef += stepSize;

		// Early out of we are outside the pixelArray
		if (temp.x < 0.0f || temp.x >= mAssetSize || temp.y < 0.0f || temp.y >= mAssetSize)
			continue;

		if (isVertical)
			thisXY = temp.y;
		else
			thisXY = temp.x;

		if (thisXY != lastXY)
		{
			setPixel(temp, color);
			lastXY = thisXY;
		}
	}
}

void ArtGenerator::addCircle(Vector2 origin, float radius, int samples, Color* color)
{
	Vector2 lastPos = origin;
	Vector2 thisPos = origin;

	for (int i = 0; i < samples + 1; i++)
	{
		thisPos = Vector2(
			origin.x + radius * cos(i / (samples / 2.0f) * M_PI), 
			origin.y + radius * sin(i / (samples / 2.0f) * M_PI)
		);

		if (i != 0)
			addLine(lastPos, thisPos, color);
		lastPos = thisPos;
	}
}

void ArtGenerator::addShape(std::vector<Vector2> points, Color* lineColor, Color* fillColor /*= nullptr*/)
{
	Vector2 origPt = points[0];

	for (int i = 0; i < points.size(); i++)
	{
		addLine(points[i], points[(i + 1) % points.size()], lineColor);
	}

	if (fillColor != nullptr)
	{
		Vector2 avgPt = Vector2(0, 0);
		for (Vector2 pt : points)
			avgPt += pt;
		avgPt = avgPt / points.size();
		avgPt = Vector2(round(avgPt.x), round(avgPt.y));
		ArtGenerator::fillColor(avgPt, fillColor);
	}
}

void ArtGenerator::fillColor(Vector2 origin, Color* destColor, Color* origColor /*= nullptr*/)
{
	if (origColor == nullptr)
		origColor = mPixelArray[origin.x][origin.y];

	std::set<Vector2> likeValidNeighbours;
	std::set<Vector2> toSearch;
	toSearch.insert(origin);

	while (toSearch.size() > 0)
	{
		findLikeNeighbours(*toSearch.begin(), likeValidNeighbours, toSearch, origColor);
	} 

	for (std::set<Vector2>::iterator it = likeValidNeighbours.begin(); it != likeValidNeighbours.end(); it++)
		setPixel(*it, destColor);
}

void ArtGenerator::resetPixelArray()
{
	mPixelArray.resize(mAssetSize);
	for (int i = 0; i < mPixelArray.size(); i++)
	{
		mPixelArray[i].resize(mAssetSize);
		for (int j = 0; j < mPixelArray[i].size(); j++)
		{
			mPixelArray[i][j] = nullptr;
		}
	}
}

void ArtGenerator::pivotPixelArray()
{
	std::vector<std::vector<Color*>> newPixelArray;
	newPixelArray.resize(mAssetSize);
	for (int x = 0; x < newPixelArray.size(); x++)
	{
		newPixelArray[x].resize(mAssetSize);
		for (int y = 0; y < newPixelArray[x].size(); y++)
			newPixelArray[x][y] = mPixelArray[y][x];
	}

	mPixelArray = newPixelArray;
}

void ArtGenerator::findLikeNeighbours(Vector2 origin, std::set<Vector2> &validNeighbours, std::set<Vector2> &toSearch, Color* origColor /*= nullptr*/)
{
	int x = origin.x;
	int y = origin.y;

	if (origColor == nullptr)
		validNeighbours.insert(origin);

	//up
	if(y != 0)
		if(mPixelArray[x][y - 1] == origColor)
			if (validNeighbours.find(Vector2(x, y - 1)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x, y - 1));
				toSearch.insert(Vector2(x, y - 1));
			}

	//down
	if (y != mAssetSize - 1)
		if (mPixelArray[x][y + 1] == origColor)
			if (validNeighbours.find(Vector2(x, y + 1)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x, y + 1));
				toSearch.insert(Vector2(x, y + 1));
			}

	//up
	if (x != 0)
		if (mPixelArray[x - 1][y] == origColor)
			if (validNeighbours.find(Vector2(x - 1, y)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x - 1, y));
				toSearch.insert(Vector2(x - 1, y));
			}

	//down
	if (x != mAssetSize - 1)
		if (mPixelArray[x + 1][y] == origColor)
			if (validNeighbours.find(Vector2(x + 1, y)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x + 1, y));
				toSearch.insert(Vector2(x + 1, y));
			}

	if (toSearch.find(origin) != toSearch.end())
		toSearch.erase(origin);

}	

}

