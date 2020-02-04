#define _USE_MATH_DEFINES

#include "RockGenerator.h"

#include <math.h>
#include <algorithm>

namespace godot
{

RockGenerator::RockGenerator()
	: ArtGenerator()
	, mMinPts(8)
	, mMaxPts(12)
	, mMinRadius(18)
	, mMaxRadius(22)
{
	setup();
	buildRock();
}


RockGenerator::~RockGenerator()
{

}

void RockGenerator::_register_methods()
{
	register_method((char*)"_draw", &draw);
}

void RockGenerator::_init()
{

}

void RockGenerator::draw()
{
	ArtGenerator::draw();
}

void RockGenerator::buildRock()
{
	resetPixelArray();

	mPoints.clear();

	int numPoints = rand() % (mMaxPts - mMinPts) + mMinPts;
	Vector2 origin = Vector2((mAssetSize - 1) / 2, mAssetSize - 1);

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

	for (int i = 0; i < mPoints.size() - 1; i++)
	{
		addLine(mPoints[i], mPoints[i + 1], mColorRamp[0]);
	}

	fillColor(origin, mColorRamp[1]);


	//Array points = rockGen->getPoints();
	for (int i = 1; i < mPoints.size(); i++)
	{
		Array pointsOnLine = getLine(mPoints[i - 1], mPoints[i]);
		for (int j = 1; j < pointsOnLine.size(); j++)
		{
			//sprayPixel(pointsOnLine[j], 12, .03, new Color(0.49, 0.48, 0.48), true);
			//sprayPixel(pointsOnLine[j], 6, .05, new Color(0.49, 0.48, 0.48), true);
			//sprayPixel(pointsOnLine[j], 3, .08, new Color(0.49, 0.48, 0.48), true);

			//sprayPixel(pointsOnLine[j], 4, .08, new Color(0.65, 0.62, 0.60), true);
			//sprayPixel(pointsOnLine[j], 2, .7, new Color(0.65, 0.62, 0.60), true);

			sprayPixel(pointsOnLine[j], 12, .06, mColorRamp[2], true);
			sprayPixel(pointsOnLine[j], 6, .4, nullptr, true);
			sprayPixel(pointsOnLine[j], 3, .4, nullptr, true);

			sprayPixel(pointsOnLine[j], 2, .7, nullptr, true);
		}
	}

	//Array pointsOnLine = getLine(mPoints[(rand() % 2) == 0 ? floor(mPoints.size() / 2.0) : ceil(mPoints.size() / 2.0)], Vector2(31, 63));
	for (int i = 0; i < rand()%2 + 3; i++)
	{
		Array pointsOnLine = getLine(mPoints[(rand() % (mPoints.size()-2) + 1)], Vector2(rand() % 20 + 21, 63));
		for (int j = 1; j < pointsOnLine.size(); j++)
		{
			sprayPixel(pointsOnLine[j], 6, .04, mColorRamp[2], true);
			sprayPixel(pointsOnLine[j], 3, .1, nullptr, true);
			sprayPixel(pointsOnLine[j], 2, .15, nullptr, true);
			sprayPixel(pointsOnLine[j], 1, .2, nullptr, true);
		}
	}
	
}

}


