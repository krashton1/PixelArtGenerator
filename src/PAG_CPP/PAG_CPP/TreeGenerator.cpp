#include "TreeGenerator.h"


namespace godot
{


TreeGenerator::TreeGenerator()
	: ArtGenerator()
	, mMinHeight(47)
	, mMaxHeight(48)
	, mMinNodes(6)
	, mMaxNodes(10)
	, mMinDistBetweenNodes(5)
	, mMaxDistBetweenNodes(7)
	, mMaxOffsetFromCenter(2)
	, mMinBranch(1)
	, mMaxBranch(3)
{
	setup();
	buildTree();
}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	ArtGenerator::setup(pos, size, numPixels);

	mDebugColor = new Color(1, 0, 0);

	mTrunkColorRamp.push_back(new Color(0.275, 0.125, 0.125));
	mTrunkColorRamp.push_back(new Color(0.400, 0.200, 0.200));
	mTrunkColorRamp.push_back(new Color(0.600, 0.275, 0.155));

	mLeafColorRamp.push_back(new Color(0.175, 0.410, 0.255));
	mLeafColorRamp.push_back(new Color(0.315, 0.610, 0.295));
	mLeafColorRamp.push_back(new Color(0.490, 0.805, 0.355));
}

void TreeGenerator::_register_methods()
{
	register_method((char*)"_draw", &_draw);
}

void TreeGenerator::_init()
{
	// Do nothing
}

void TreeGenerator::_draw()
{
	ArtGenerator::_draw();
}

void TreeGenerator::buildTree()
{
	// Reset Tree
	resetPixelArray();
	mNodes.clear();
	mLeaves.clear();

	// Determine height, num Nodes
	Vector2 origin = Vector2((mAssetSize - 1) / 2, mAssetSize - 1);
	int height = rand() % (mMaxHeight - mMinHeight) + mMinHeight;
	int numNodes = rand() % (mMaxNodes - mMinNodes) + mMinNodes;
	int numBranches = rand() % (mMaxBranch - mMinBranch) + mMinBranch;

	// Determine Nodes (height between, width)
	int lastHeight = 0;
	int thisHeight = 0;

	float maxWidth = 7;

	for (int i = 0; i < numNodes; i++)
	{
		//int offsetBetweenNodes = rand() % (mMaxDistBetweenNodes - mMinDistBetweenNodes) + mMinDistBetweenNodes;
		int offsetBetweenNodes = float(height) / numNodes;

		if (i == 0)
			thisHeight = 0;
		else
			thisHeight = lastHeight + offsetBetweenNodes;

		int thisWidth = floor(maxWidth - (float(i) / numNodes) * maxWidth);

		int offsetFromCenter = rand() % (mMaxOffsetFromCenter - (-mMaxOffsetFromCenter)) + (-mMaxOffsetFromCenter);
		
		Vector2 leftPt = Vector2((origin.x + offsetFromCenter - thisWidth / 2), (origin.y - thisHeight));
		Vector2 rightPt = Vector2((origin.x + offsetFromCenter + thisWidth / 2 + 1), (origin.y - thisHeight));

		setPixel(leftPt, mDebugColor);
		setPixel(rightPt, mDebugColor);

		mNodes.push_back(leftPt);
		mNodes.push_back(rightPt);

		

		lastHeight = thisHeight;
	}



	for (int i = 2; i < mNodes.size(); i += 2) 
	{
		addLine(mNodes[i - 2], mNodes[i], mDebugColor);
		addLine(mNodes[i - 1], mNodes[i + 1], mDebugColor);
	}

	Vector2 lastPoint = origin;
	Vector2 thisPoint;
	for (int i = 0; i < mNodes.size(); i += 2)
	{
		thisPoint = Vector2((static_cast<Vector2>(mNodes[i]).x + static_cast<Vector2>(mNodes[i + 1]).x) / 2, static_cast<Vector2>(mNodes[i]).y);
		fillColor(thisPoint, mDebugColor);
		addLine(lastPoint, thisPoint, mDebugColor);
		lastPoint = thisPoint;

	}

	

	// Determine branch nodes, 2 per branch
	
	// Color Trunk and branches

	// Color Leaves
}

}