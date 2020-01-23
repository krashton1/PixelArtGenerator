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
		addLine(mPoints[i], mPoints[i + 1], new Color(0, 0, 0, 1));
	}

	fillColor(origin, new Color(1, 1, 1, 1));
}

}


