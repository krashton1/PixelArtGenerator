#define _USE_MATH_DEFINES

#include "ArtGenerator.h"

#include <cmath>
#include <algorithm>

namespace godot
{

ArtGenerator::ArtGenerator(Vector2 size /*= Vector2(1024,1024)*/, int numPixels /*= 64*/) 
	: mSize(size)
	, mAssetSize(numPixels)
	, mScreenSize(Vector2(1280, 720)) 
	, mPixelSize(std::min(size.x, size.y) / numPixels)
{

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
	setup();
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
	for (int i = 0; i < mAssetSize; i++) 
	{
		for (int j = 0; j < mAssetSize; j++)
		{
			if (mPixelArray[i][j] != nullptr)
			{
				drawPixel(Vector2(i*mPixelSize, j*mPixelSize), mPixelArray[i][j]);
			}
		}
	}
}

void ArtGenerator::setPixel(Vector2 pos, Color* color, int flag /*= 0*/)
{
	// flag == 0 : force pixel to be colored
	// flag == 1 : only change pixel if it was previously colored
	// flag == 2 : only change pixel if it was NOT previously colored

	if (flag == 0)
		mPixelArray[(int)pos.x][(int)pos.y] = color;
	else if (flag == 1)
		if (mPixelArray[(int)pos.x][(int)pos.y] != nullptr && !compareColor(mPixelArray[(int)pos.x][(int)pos.y], mBlack))
			mPixelArray[(int)pos.x][(int)pos.y] = color;
	else if (flag == 2)
		if (mPixelArray[(int)pos.x][(int)pos.y] == nullptr)
			mPixelArray[(int)pos.x][(int)pos.y] = color;
		
	
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
	int lastXY = -1;
	int thisXY = -1;

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
		origColor = mPixelArray[(int)origin.x][(int)origin.y];

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

void ArtGenerator::sprayPixel(Vector2 origin, float size, float intensity, Color* color, bool paintOver /*= false*/)
{
	// Circle of radius size located at origin
	// intensity 0 means no pixels are colored
	// intensity of 1 means all pixels are colored
	// Does pixels closer to center get colored more often? thinking no after gut feeling said yes

	Vector2 start = origin - Vector2(size, size);

	for (int x = start.x; x <= origin.x + size; x++)
	{
		for (int y = start.y; y <= origin.y + size; y++)
		{
			//setPixel(Vector2(x, y), new Color(1, 0, 0));

			if (x < 0 || x > 63 || y < 0 || y > 63)
				continue;

			float dist = std::sqrt(std::pow(x - origin.x, 2) + std::pow(y- origin.y, 2));
			if (dist <= size)
			{
				//setPixel(Vector2(x, y), new Color(0, 1, 0));

				float t = rand() / (float)RAND_MAX;
				if (t < intensity)
					setPixel(Vector2(x, y), color, (paintOver ? 1 : 0));
			}


		}
	}

}

void ArtGenerator::resetPixelArray()
{
	for (int i = 0; i < mAssetSize; i++)
	{
		for (int j = 0; j < mAssetSize; j++)
		{
			mPixelArray[i][j] = nullptr;
		}
	}
}

void ArtGenerator::pivotPixelArray()
{
	Color* newPixelArray[64][64];

	for (int x = 0; x < mAssetSize; x++)
	{
		for (int y = 0; y < mAssetSize; y++)
			newPixelArray[x][y] = mPixelArray[y][x];
	}

	for (int x = 0; x < mAssetSize; x++)
	{
		for (int y = 0; y < mAssetSize; y++)
			mPixelArray[x][y] = newPixelArray[x][y];
	}
}

godot::Array ArtGenerator::getLine(Vector2 origin, Vector2 dest)
{
	Array pointsOnLine;

	float stepSize = 1.0f / 64;

	Vector2 dir = dest - origin;

	// If a line is more vertical than horizontal, it should not have more than 1 pixel in same y coord. aka jaggies
	bool isVertical;
	if (abs(dir.y) >= abs(dir.x))
		isVertical = true;
	else
		isVertical = false;

	Vector2 temp = origin;

	// Holds prev XY coord, depending on if line is horizontal or vertical, which is then used to remove jaggies
	int lastXY = -1;
	int thisXY = -1;

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
		if (temp.x < 0.0f || temp.x >= 64 || temp.y < 0.0f || temp.y >= 64)
			continue;

		if (isVertical)
			thisXY = temp.y;
		else
			thisXY = temp.x;

		if (thisXY != lastXY)
		{
			pointsOnLine.append(temp);
			lastXY = thisXY;
		}
	}

	return pointsOnLine;
}

bool ArtGenerator::compareColor(Color* color1, Color* color2)
{

	if (color1->r == color2->r && color1->g == color2->g && color1->b == color2->b && color1->a == color2->a)
		return true;

	return false;
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

void ArtGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	mPosition = pos;
	mSize = size;
	mAssetSize = numPixels;
	mScreenSize = Vector2(1280, 720);
	mPixelSize = std::min(mSize.x, mSize.y) / numPixels;

	mBlack = new Color(0.2, 0.2, 0.2);
	
	resetPixelArray();

	//addLine(Vector2(0, 0), Vector2(20, 10), new Color(0.0, 0.0, 0.0, 1.0));
}

}

