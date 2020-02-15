#include "TreeGenerator.h"
#include <algorithm>


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


	mColorRamp.push_back(new Color(0.20, 0.20, 0.20));

	mTrunkColorRamp.push_back(new Color(0.275, 0.125, 0.125));
	mTrunkColorRamp.push_back(new Color(0.450, 0.175, 0.175));
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

	float maxWidth = 6;

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

		setPixel(leftPt, mTrunkColorRamp[1]);
		setPixel(rightPt, mTrunkColorRamp[1]);

		mNodes.push_back(leftPt);
		mNodes.push_back(rightPt);

		lastHeight = thisHeight;
	}


	mLeaves.append(mNodes[mNodes.size() - 1]);

	for (int i = 2; i < mNodes.size(); i += 2) 
	{
		addLine(mNodes[i - 2], mNodes[i], mTrunkColorRamp[1]);
		addLine(mNodes[i - 1], mNodes[i + 1], mTrunkColorRamp[1]);
	}

	Vector2 lastPoint = origin;
	Vector2 thisPoint;
	for (int i = 0; i < mNodes.size(); i += 2)
	{
		thisPoint = Vector2((static_cast<Vector2>(mNodes[i]).x + static_cast<Vector2>(mNodes[i + 1]).x) / 2, static_cast<Vector2>(mNodes[i]).y);
		fillColor(thisPoint, mTrunkColorRamp[1]);
		addLine(lastPoint, thisPoint, mTrunkColorRamp[1]);
		lastPoint = thisPoint;

	}

	for (int i = 2; i < mNodes.size(); i += 2)
	{
		int leftBot = static_cast<Vector2>(mNodes[i - 2]).x;
		int rightBot = static_cast<Vector2>(mNodes[i - 1]).x;

		int leftTop = static_cast<Vector2>(mNodes[i]).x;
		int rightTop = static_cast<Vector2>(mNodes[i + 1]).x;

		for (int j = 0; j < 3; j++)
		{
			//int botX = rand() % (rightBot - leftBot + 2) + leftBot;
			//int topX = rand() % (rightTop - leftTop + 2) + leftTop;
			//topX = std::max(std::min(topX, botX + 2), botX - 2);

			int botX = rightBot - j * (rand() % 2 + 2);
			int topX = rightTop - j * (rand() % 2 + 2);

			Vector2 botPt = Vector2(botX, static_cast<Vector2>(mNodes[i - 2]).y);
			Vector2 topPt = Vector2(topX, static_cast<Vector2>(mNodes[i - 0]).y);

			addLine(botPt, topPt, mTrunkColorRamp[2], 1);

		}
	}





	for (int i = 2; i < mNodes.size(); i += 2)
	{
		int leftBot = static_cast<Vector2>(mNodes[i - 2]).x;
		int rightBot = static_cast<Vector2>(mNodes[i - 1]).x;

		int leftTop = static_cast<Vector2>(mNodes[i]).x;
		int rightTop = static_cast<Vector2>(mNodes[i + 1]).x;

		for (int j = 0; j < 3; j++)
		{
			//int botX = rand() % (rightBot - leftBot + 2) + leftBot;
			//int topX = rand() % (rightTop - leftTop + 2) + leftTop;
			//topX = std::max(std::min(topX, botX + 2), botX - 2);

			int botX = leftBot + j * (rand() % 2 + 2);
			int topX = leftTop + j * (rand() % 2 + 2);

			Vector2 botPt = Vector2(botX, static_cast<Vector2>(mNodes[i - 2]).y);
			Vector2 topPt = Vector2(topX, static_cast<Vector2>(mNodes[i - 0]).y);

			addLine(botPt, topPt, mTrunkColorRamp[0], 1);

		}
	}



	sprayPixel(Vector2(31, 31), 32, 0.1, mTrunkColorRamp[2], true);
	sprayPixel(Vector2(31, 31), 32, 0.1, mTrunkColorRamp[0], true);



	std::set<int> usedNodes;

	for (int i = 0; i <= numBranches; i++)
	{
		
		int buildOffNode = 0;
		
		while (buildOffNode == 0) 
		{
			int t = rand() % (mNodes.size() - 8) + 4;
			if (usedNodes.find(t) == usedNodes.end())
			{
				usedNodes.insert(t);
				buildOffNode = t;
			}
		}


		int branchLength = rand() % 4 + 12 - std::min(buildOffNode, 7);

		Vector2 branchEnd = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * branchLength, static_cast<Vector2>(mNodes[buildOffNode]).y - (rand() % 3 + 2));

		Vector2 branchMid = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * (branchLength - 3), static_cast<Vector2>(mNodes[buildOffNode]).y - 1);

		setPixel(branchEnd, mTrunkColorRamp[1]);
		setPixel(branchMid, mTrunkColorRamp[1]);

		addLine(branchEnd, branchMid + Vector2((buildOffNode % 2 == 1 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), mTrunkColorRamp[1]);
		addLine(branchEnd, branchMid, mTrunkColorRamp[1]);
		addLine(branchEnd, branchMid + Vector2((buildOffNode % 2 == 0 ? 1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), mTrunkColorRamp[1]);

		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 0), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 1), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 2), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 3), mTrunkColorRamp[1]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 0), mTrunkColorRamp[1]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 1), mTrunkColorRamp[1]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 2), mTrunkColorRamp[1]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 3), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 0), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 1), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 2), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 3), mTrunkColorRamp[1]);
	
	
		mLeaves.append(branchEnd);
	
	
	
	}




	



	for (int i = 0; i < mLeaves.size(); i++)
	{
		Vector2 currentLeaf = static_cast<Vector2>(mLeaves[i]);

		//addCircle(currentLeaf, 3, 12, debugColor2);
		sprayPixel(currentLeaf, 12, 0.7, mLeafColorRamp[0]);
		sprayPixel(currentLeaf, 8, 0.6, mLeafColorRamp[0]);
		sprayPixel(currentLeaf, 6, 0.6, mLeafColorRamp[0]);

		sprayPixel(currentLeaf + Vector2(0, 0), 12, 0.2, mLeafColorRamp[1], true);
		sprayPixel(currentLeaf + Vector2(-2, -2), 10, 0.6, mLeafColorRamp[1]);

		sprayPixel(currentLeaf + Vector2(-4, -4), 10, 0.4, mLeafColorRamp[2], true);
	}

	



	

	// Determine branch nodes, 2 per branch
	
	// Color Trunk and branches

	// Color Leaves
}

}