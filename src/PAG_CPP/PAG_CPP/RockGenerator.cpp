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
	register_method((char*)"_draw", &_draw);
}

void RockGenerator::_init()
{
	// Do nothing
}

void RockGenerator::_draw()
{
	ArtGenerator::_draw();
}

void RockGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	ArtGenerator::setup(pos, size, numPixels);

	mColorRamp.push_back(new Color(0.20, 0.20, 0.20));
	mColorRamp.push_back(new Color(0.30, 0.30, 0.30));
	mColorRamp.push_back(new Color(0.40, 0.40, 0.40));
	mColorRamp.push_back(new Color(0.45, 0.45, 0.45));
	mColorRamp.push_back(new Color(0.50, 0.50, 0.50));
	mColorRamp.push_back(new Color(0.60, 0.60, 0.60));
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


