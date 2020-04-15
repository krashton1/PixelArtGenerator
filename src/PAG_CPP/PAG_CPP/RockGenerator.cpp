#define _USE_MATH_DEFINES

#include "RockGenerator.h"
#include <math.h>
#include <algorithm>

namespace godot
{

RockGenerator::RockGenerator()
	: AssetGenerator()
	, mMinPts(8)
	, mMaxPts(12)
	, mMinRadius(18)
	, mMaxRadius(22)
{
	setup();
	//buildRock();
}

RockGenerator::~RockGenerator()
{

}

void RockGenerator::_register_methods()
{
	register_method((char*)"_draw", &_draw);
}

void RockGenerator::_init()
{
	// Do nothing
}

void RockGenerator::_draw()
{
	AssetGenerator::_draw();
}

void RockGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	AssetGenerator::setup(pos, size, numPixels);

	//setType(RockTypeGraphite);
}

void RockGenerator::setType(RockType rockType)
{
	mRockType = rockType;
	mColorRamp.clear();

	if (mRockType == RockTypeGraphite)
	{
		mDensity = 1.0;

		mColorRamp.push_back(new Color(0.20, 0.20, 0.20));
		mColorRamp.push_back(new Color(0.30, 0.30, 0.30));
		mColorRamp.push_back(new Color(0.40, 0.40, 0.40));
		mColorRamp.push_back(new Color(0.45, 0.45, 0.45));
		mColorRamp.push_back(new Color(0.50, 0.50, 0.50));
		mColorRamp.push_back(new Color(0.60, 0.60, 0.60));
	}
	else if (mRockType == RockTypeBush)
	{
		mDensity = 1.0;

		mColorRamp.push_back(new Color(0.20, 0.20, 0.20));
		mColorRamp.push_back(new Color(0.30, 0.30, 0.30));
		mColorRamp.push_back(new Color(0.175, 0.410, 0.255));
		mColorRamp.push_back(new Color(0.315, 0.610, 0.295));
		mColorRamp.push_back(new Color(0.490, 0.805, 0.355));
	}
	else if (mRockType == RockTypeCloud)
	{
		mDensity = 1.0;

		mColorRamp.push_back(new Color(0.7, 0.7, 0.7, 0.5));
		mColorRamp.push_back(new Color(0.8, 0.8, 0.8, 0.6));
		mColorRamp.push_back(new Color(0.85, 0.85, 0.85, 0.7));
		mColorRamp.push_back(new Color(0.9, 0.9, 0.9, 0.8));
		mColorRamp.push_back(new Color(0.95, 0.95, 0.95, 0.9));
	}

	buildRock();
}

void RockGenerator::buildRock()
{
	// Reset rock
	resetPixelArray();
	mPoints.clear();

	// Determine variable of rock
	Vector2 origin = Vector2((mAssetSize - 1) / 2, mAssetSize - 1);
	int numPoints = rand() % (mMaxPts - mMinPts) + mMinPts;

	// Find vertices along edge of rock
	for (int i = 0; i < numPoints + 1; i++)
	{
		int thisRadius = rand() % (mMaxRadius - mMinRadius) + mMinRadius;
		float thisAngle = M_PI / float(numPoints) * float(i);

		Vector2 thisPoint = Vector2(
			origin.x + thisRadius * cos(thisAngle),
			origin.y + thisRadius * sin(M_PI + thisAngle)
		);

		mPoints.push_back(thisPoint);

	}

	if (mRockType == RockTypeGraphite)
	{
		// Connect Vertices
		for (int i = 0; i < mPoints.size() - 1; i++)
		{
			addLine(mPoints[i], mPoints[i + 1], mColorRamp[0]);
		}

		// Fill rock in
		fillColor(origin, mColorRamp[1]);



		// Create faces along edge of rock
		for (int i = 1; i < mPoints.size(); i++)
		{
			Array pointsOnLine = getLine(mPoints[i - 1], mPoints[i]);
			for (int j = 1; j < pointsOnLine.size(); j++)
			{
				sprayPixel(pointsOnLine[j], 12, .06, mColorRamp[2], true);

				sprayPixel(pointsOnLine[j], 6, .4, nullptr, true);
				sprayPixel(pointsOnLine[j], 3, .4, nullptr, true);
				sprayPixel(pointsOnLine[j], 2, .7, nullptr, true);
			}
		}


		// Create faces through body of rock
		for (int i = 0; i < rand() % 2 + 3; i++)
		{
			Array pointsOnLine = getLine(mPoints[(rand() % (mPoints.size() - 2) + 1)], Vector2(rand() % 20 + 21, mAssetSize));
			for (int j = 1; j < pointsOnLine.size(); j++)
			{
				sprayPixel(pointsOnLine[j], 6, .04, mColorRamp[2], true);

				sprayPixel(pointsOnLine[j], 3, .1, nullptr, true);
				sprayPixel(pointsOnLine[j], 2, .15, nullptr, true);
				sprayPixel(pointsOnLine[j], 1, .2, nullptr, true);
			}
		}

		if (mBlur)
			blurPixels();
	}
	else if (mRockType == RockTypeBush)
	{
		for (int i = 1; i < mPoints.size() - 1; i++)
		{
			Array pointsOnLine = getLine(mPoints[(rand() % (mPoints.size() - 2) + 1)], Vector2(rand() % 20 + 21, mAssetSize));
			for (int j = 1; j < pointsOnLine.size(); j++)
			{

				//int size = rand() % 5 + 4;
				//int size = std::max(std::min(rand() % int((63-currentLeaf.y) / 3) + 2, 8), 4);

				int size;// = int(54 - currentLeaf.y) / 5 + 4;

				size = 5;


				//addCircle(currentLeaf, 3, 12, debugColor2);
				sprayPixel(pointsOnLine[j], floor(size * 2.0), 0.10, mColorRamp[2]);
				sprayPixel(pointsOnLine[j], floor(size * 1.5), 0.05, mColorRamp[2]);

				sprayPixel(Vector2(-1, -1) + pointsOnLine[j], floor(size * 2.0), 0.15, mColorRamp[3], true);

				sprayPixel(Vector2(-2, -2) + pointsOnLine[j], floor(size * 1.2), 0.2, mColorRamp[4], true);


				// cactus 

				////addCircle(currentLeaf, 3, 12, debugColor2);
				//sprayPixel(pointsOnLine[j], floor(size * 2.0), 0.7, mColorRamp[2]);
				//sprayPixel(pointsOnLine[j], floor(size * 1.5), 0.6, mColorRamp[2]);
				//sprayPixel(pointsOnLine[j], floor(size * 1.0), 0.6, mColorRamp[2]);

				//sprayPixel(Vector2(0, 0) + pointsOnLine[j], floor(size * 2.0), 0.2, mColorRamp[3], true);
				//sprayPixel(Vector2(-2, -2) + pointsOnLine[j], floor(size * 1.7), 0.6, mColorRamp[3]);

				//sprayPixel(Vector2(-4, -4) + pointsOnLine[j] + Vector2(-4, -4), floor(size * 1.7), 0.4, mColorRamp[4], true);

			}

			pointsOnLine = getLine(mPoints[i - 1], mPoints[i]);
			for (int j = 1; j < pointsOnLine.size(); j++)
			{
				int size;// = int(54 - currentLeaf.y) / 5 + 4;

				size = 5;

				sprayPixel(Vector2(-1, -1) + pointsOnLine[j], floor(size * 2.0), 0.15, mColorRamp[3], true);

				sprayPixel(Vector2(-2, -2) + pointsOnLine[j], floor(size * 1.2), 0.2, mColorRamp[4], true);
			}
		}


		if (mBlur)
			blurPixels();

		int numFlowers = rand() % 5 + 5;
		for (int i = 0; i < numFlowers; i++)
		{
			int x;
			int y;
			bool foundSpot = false;
			while (!foundSpot)
			{
				x = rand() % 30 + 15;
				y = rand() % mAssetSize;

				if (mPixelArray[x][y] != nullptr)
				{
					foundSpot = true;
				}
			}

			setPixel(Vector2(x, y), new Color(0.7, 0.1, 0.6));

			setPixel(Vector2(x, y + 1), new Color(0.9, 0.4, 0.8));
			setPixel(Vector2(x, y - 1), new Color(0.9, 0.4, 0.8));
			setPixel(Vector2(x - 1, y), new Color(0.9, 0.4, 0.8));
			setPixel(Vector2(x + 1, y), new Color(0.9, 0.4, 0.8));
		}

	}
	else if (mRockType == RockTypeCloud)
	{
		for (int i = 0; i < 12; i++)
		{
			int x = rand() % 30 + 15;
			int y = rand() % 15 + 30;

			sprayPixel(Vector2(x, y), rand() % 6 + 6, 0.75, mColorRamp[1], false);
			sprayPixel(Vector2(x, y), rand() % 6 + 6, 0.5, nullptr, true);
			sprayPixel(Vector2(x, y), rand() % 6 + 6, 0.5, nullptr, true);
			sprayPixel(Vector2(x, y), rand() % 6 + 6, 0.5, nullptr, true);
			sprayPixel(Vector2(x, y), rand() % 6 + 6, 0.5, nullptr, true);

		}


		if (mBlur)
			blurPixels();
	}

	
	
}

}


