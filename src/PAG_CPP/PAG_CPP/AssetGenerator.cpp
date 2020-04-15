#define _USE_MATH_DEFINES

#include "AssetGenerator.h"
#include <cmath>
#include <algorithm>

#include <deque>

namespace godot
{

AssetGenerator::AssetGenerator(Vector2 size /*= Vector2(1024,1024)*/, int numPixels /*= 64*/) 
	: mSize(size)
	, mAssetSize(numPixels)
	, mScreenSize(Vector2(1280, 720)) 
	, mPixelSize(std::min(size.x, size.y) / numPixels)
{
	setup();
}

AssetGenerator::~AssetGenerator()
{

}

void AssetGenerator::_register_methods()
{
	register_method((char*)"_ready", &_ready);
	register_method((char*)"_draw", &_draw);
}

void AssetGenerator::_init()
{
	// Do nothing
}

void AssetGenerator::_ready()
{
	// Do nothing
}

void AssetGenerator::_draw()
{
	//drawPixelArray();

	//int newX = -(mBandCurPos[i] * mPixelSize) + prevX;
	//Rect2 rect = Rect2(newX - mPixelSize, (i == 0 ? 0 : mBandPos[i - 1]) - mPixelSize, (mBandImages[i][x]->get_width() * mPixelSize) + mPixelSize, mBandPos[i] - (i == 0 ? 0 : mBandPos[i - 1]) + mPixelSize);
	
	mImageTexture->create_from_image(mImage, 0);
	Rect2 rect = Rect2(0, 0, mSize.x, mSize.y);

	draw_texture_rect(mImageTexture, rect, false);
}

void AssetGenerator::drawPixel(Vector2 pos, Color* color)
{
	Rect2 rect = Rect2(pos.x, pos.y, mPixelSize, mPixelSize);
	draw_rect(rect, *color);
}

void AssetGenerator::drawPixelArray()
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

void AssetGenerator::setPixel(Vector2 pos, Color* color, int flag /*= 0*/)
{
	// flag == 0 : force pixel to be colored
	// flag == 1 : only change pixel if it was previously colored
	// flag == 2 : only change pixel if it was NOT previously colored

	if (pos.x >= mAssetSize || pos.x < 0 || pos.y >= mAssetSize || pos.y < 0 || color == nullptr)
		return;

	mImage->lock();

	if (flag == 0)
	{
		mPixelArray[(int)pos.x][(int)pos.y] = color;
		mImage->set_pixel(int(pos.x), int(pos.y), *color);
	}
	else if (flag == 1)
	{
		if (mPixelArray[(int)pos.x][(int)pos.y] != nullptr && !compareColor(mPixelArray[(int)pos.x][(int)pos.y], mColorRamp[0]))
		{
			mPixelArray[(int)pos.x][(int)pos.y] = color;
			mImage->set_pixel(int(pos.x), int(pos.y), *color);
		}
	}
		
	else if (flag == 2)
	{
		if (mPixelArray[(int)pos.x][(int)pos.y] == nullptr)
		{
			mPixelArray[(int)pos.x][(int)pos.y] = color;
			mImage->set_pixel(int(pos.x), int(pos.y), *color);

		}
	}

	mImage->unlock();
}

void AssetGenerator::addLine(Vector2 origin, Vector2 dest, Color* color, int flag /*= 0*/)
{
	// flag == 0 : force pixel to be colored
	// flag == 1 : only change pixel if it was previously colored
	// flag == 2 : only change pixel if it was NOT previously colored

	Array linePoints = getLine(origin, dest);

	for (int i = 0; i < linePoints.size(); i++)
	{
		setPixel(linePoints[i], color, flag);
	}
}

void AssetGenerator::addCircle(Vector2 origin, float radius, int samples, Color* color)
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

void AssetGenerator::addShape(std::vector<Vector2> points, Color* lineColor, Color* fillColor /*= nullptr*/)
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
		AssetGenerator::fillColor(avgPt, fillColor);
	}
}

void AssetGenerator::fillColor(Vector2 origin, Color* destColor, Color* origColor /*= nullptr*/, Color* destColor2 /*= nullptr*/)
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
	{
		Color* c = destColor;
		if (destColor2 != nullptr)
			if (rand() % 2 == 0)
				c = destColor2;

		setPixel(*it, c);
	}
}

void AssetGenerator::sprayPixel(Vector2 origin, float size, float intensity, Color* color /*= nullptr*/, bool paintOver /*= false*/)
{
	// Circle of radius size located at origin
	// intensity 0 means no pixels are colored
	// intensity of 1 means all pixels are colored

	// If supplied color is null, just brighten the pixel by 1 on ramp

	Vector2 start = origin - Vector2(size, size);

	for (int x = start.x; x <= origin.x + size; x++)
	{
		for (int y = start.y; y <= origin.y + size; y++)
		{
			if (x < 0 || x >= mAssetSize || y < 0 || y >= mAssetSize)
				continue;

			float dist = std::sqrt(std::pow(x - origin.x, 2) + std::pow(y- origin.y, 2));
			if (dist <= size)
			{
				float t = rand() / (float)RAND_MAX;
				if (t < intensity)
				{
					if (color != nullptr)
					{
						setPixel(Vector2(x, y), color, (paintOver ? 1 : 0));
					}
					else
					{
						Color* curColor = mPixelArray[x][y];
						Color* tempColor = mColorRamp[0];
						int i = 0;

						while (true)
						{
							if (compareColor(curColor, tempColor))
							{
								setPixel(Vector2(x, y), mColorRamp[std::min(i + 1, (int)mColorRamp.size() - 1)], (paintOver ? 1 : 0));
								break;
							}
							i++;

							if (i == mColorRamp.size())
								break;

							tempColor = mColorRamp[i];
						}
					}
				}
			}
		}
	}
}

void AssetGenerator::blurPixels()
{

	Color* newPixelArray[64][64];

	for (int i = 0; i < mAssetSize; i++)
	{
		for (int j = 0; j < mAssetSize; j++)
		{
			newPixelArray[i][j] = nullptr;
		}
	}

	for (int x = 0; x < mAssetSize ; x++)
	{
		for (int y = 0; y < mAssetSize; y++)
		{
			int n = 0;

			Vector3 avgColor = Vector3(0, 0, 0);
			float alpha = 0.0;

			if (mPixelArray[x][y] != nullptr)
			{
				avgColor = avgColor + Vector3(mPixelArray[x][y]->r, mPixelArray[x][y]->g, mPixelArray[x][y]->b);
				n++;
				alpha = 1.0;
			}
			if (x - 1 >= 0 && mPixelArray[x-1][y] != nullptr)
			{
				avgColor = avgColor + Vector3(mPixelArray[x - 1][y]->r, mPixelArray[x - 1][y]->g, mPixelArray[x - 1][y]->b);
				n++;
			}
			if (x + 1 < mAssetSize && mPixelArray[x+1][y] != nullptr)
			{
				avgColor = avgColor + Vector3(mPixelArray[x + 1][y]->r, mPixelArray[x + 1][y]->g, mPixelArray[x + 1][y]->b);
				n++;
			}
			if (y - 1 >= 0 && mPixelArray[x][y-1] != nullptr)
			{
				avgColor = avgColor + Vector3(mPixelArray[x][y - 1]->r, mPixelArray[x][y - 1]->g, mPixelArray[x][y - 1]->b);
				n++;
			}
			if (y + 1 < mAssetSize && mPixelArray[x][y+1] != nullptr)
			{
				avgColor = avgColor + Vector3(mPixelArray[x][y + 1]->r, mPixelArray[x][y + 1]->g, mPixelArray[x][y + 1]->b);
				n++;
			}

			if (n != 0)
			{
				avgColor = avgColor / n;

				Color* newColor = new Color(avgColor.x, avgColor.y, avgColor.z, alpha);
				newPixelArray[x][y] = newColor;

			}

		}
	}


	for (int x = 0; x < mAssetSize; x++)
	{
		for (int y = 0; y < mAssetSize; y++)
		{
			if (newPixelArray[x][y] != nullptr)
				setPixel(Vector2(x, y), newPixelArray[x][y]);
		}
	}
}

void AssetGenerator::resetPixelArray()
{
	mImage.instance();
	mImage->create(mAssetSize, mAssetSize, false, Image::Format::FORMAT_RGBA8);

	mImageTexture.instance();

	for (int i = 0; i < mAssetSize; i++)
	{
		for (int j = 0; j < mAssetSize; j++)
		{
			mPixelArray[i][j] = nullptr;
		}
	}
}

void AssetGenerator::rotatePixelArray()
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

Array AssetGenerator::getLine(Vector2 origin, Vector2 dest)
{
	Array pointsOnLine;

	float stepSize = 1.0f / mAssetSize; // todo fix this

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
		if (temp.x < 0.0f || temp.x >= mAssetSize || temp.y < 0.0f || temp.y >= mAssetSize) // todo fix this
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

bool AssetGenerator::compareColor(Color* color1, Color* color2)
{
	if (color1 == nullptr || color2 == nullptr)
		return false;

	if (color1->r == color2->r && color1->g == color2->g && color1->b == color2->b && color1->a == color2->a)
		return true;

	return false;
}

void AssetGenerator::addSmile()
{
	setPixel(Vector2(0, 1), mDebugColor);
	setPixel(Vector2(0, 3), mDebugColor);
	setPixel(Vector2(2, 0), mDebugColor);
	setPixel(Vector2(2, 4), mDebugColor);
	setPixel(Vector2(3, 1), mDebugColor);
	setPixel(Vector2(3, 2), mDebugColor);
	setPixel(Vector2(3, 3), mDebugColor);

	rotatePixelArray();
}

void AssetGenerator::addMountain(int height, Color* color1, Color* color2)
{

	resetPixelArray();

	if (height < 2)
		return;

	Vector2 botLeft = Vector2(0, mAssetSize - 1);
	Vector2 botRight = Vector2(mAssetSize - 1, mAssetSize - 1);
	int centerX = rand() % 20 + (mAssetSize - 20) / 2;
	int centerY = mAssetSize - height + (rand() % (height/2)) - height/4;
	if (centerY >= mAssetSize - 1)
		centerY = 1;
	Vector2 peak = Vector2(centerX, centerY);

	std::deque<Vector2> peaks;

	//peaks.push_back(botLeft);
	peaks.push_back(peak);
	//peaks.push_back(botRight);


	int smallerPeaks = (height - 10) / 2;

	for (int i = 0; i < smallerPeaks; i++)
	{

		int smallPeakX = rand() % 50 + (mAssetSize - 50) / 2;
		int variationY = (rand() % height /3) + ((peak.x - smallPeakX) > 0 ? (peak.x - smallPeakX) : (smallPeakX - peak.x));
		int smallPeakY = mAssetSize - height + variationY;
		if (smallPeakY >= mAssetSize - 1)
			smallPeakY = mAssetSize - 2;
		Vector2 smallPeak = Vector2(smallPeakX, smallPeakY);

		//if (smallPeak.x < peak.x - 5)
		//{
		//	peaks.push_front(smallPeak);
		//}
		//else if (smallPeak.x > peak.x + 5)
		//{
		peaks.push_back(smallPeak);
		//}


	}


	peaks.push_front(botLeft);
	peaks.push_back(botRight);


	std::deque<Vector2> peaksSorted;

	std::set<int> doneX;

	for (int i = 0; i < peaks.size(); i++)
	{
		int nextX = mAssetSize;
		int nextIndex = -1;

		for (int j = 0; j < peaks.size(); j++)
		{
			if (peaks[j].x < nextX && doneX.find(peaks[j].x) == doneX.end())
			{
				nextX = peaks[j].x;
				nextIndex = j;
			}
		}

		if (nextIndex != -1)
		{

			doneX.insert(peaks[nextIndex].x);
			peaksSorted.push_back(peaks[nextIndex]);
		}
	}

	for (int i = 1; i < peaksSorted.size(); i++)
	{
		addLine(peaksSorted[i - 1], peaksSorted[i], color1);
	}


	fillColor(Vector2(centerX, mAssetSize - 1), color1, nullptr, color2);


	if (mPixelArray[0][0] != nullptr)
	{
		addMountain(height, color1, color2);
		return;
	}

	//for (int i = 0; i < peaksSorted.size(); i++)
	//{
	//	if (mPixelArray[int(peaksSorted[i].x - 1)][int(peaksSorted[i].y)] == nullptr && mPixelArray[int(peaksSorted[i].x + 1)][int(peaksSorted[i].y)] == nullptr)
	//	{
	//		setPixel(peaksSorted[i], new Color(0, 0, 0, 0));

	//		setPixel(peaksSorted[i] + Vector2(0, 1), color1);
	//	}
	//}

	if (mBlur)
		blurPixels();








}

void AssetGenerator::findLikeNeighbours(Vector2 origin, std::set<Vector2> &validNeighbours, std::set<Vector2> &toSearch, Color* origColor /*= nullptr*/)
{
	int x = origin.x;
	int y = origin.y;

	if (origColor == nullptr)
		validNeighbours.insert(origin);

	// up
	if(y != 0)
		if(mPixelArray[x][y - 1] == origColor)
			if (validNeighbours.find(Vector2(x, y - 1)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x, y - 1));
				toSearch.insert(Vector2(x, y - 1));
			}

	// down
	if (y != mAssetSize - 1)
		if (mPixelArray[x][y + 1] == origColor)
			if (validNeighbours.find(Vector2(x, y + 1)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x, y + 1));
				toSearch.insert(Vector2(x, y + 1));
			}

	// left
	if (x != 0)
		if (mPixelArray[x - 1][y] == origColor)
			if (validNeighbours.find(Vector2(x - 1, y)) == validNeighbours.end())
			{
				validNeighbours.insert(Vector2(x - 1, y));
				toSearch.insert(Vector2(x - 1, y));
			}

	// right
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

void AssetGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	mPosition = pos;
	mSize = size;
	mAssetSize = numPixels;
	mScreenSize = Vector2(1280, 720);
	mPixelSize = std::min(mSize.x, mSize.y) / numPixels;
	mDebugColor = new Color(0, 0, 0);

	resetPixelArray();


	//mImage.instance();
	//mImage->create(mAssetSize, mAssetSize, false, Image::Format::FORMAT_RGBA8);

	//mImageTexture.instance();
}

}

