#include "TreeGenerator.h"
#include <algorithm>


namespace godot
{


	// Deciduous

	//  , mMinHeight(32)
	//	, mMaxHeight(44)
	//	, mMinNodes(6)
	//	, mMaxNodes(10)
	//	, mMinDistBetweenNodes(5)
	//	, mMaxDistBetweenNodes(7)
	//	, mMaxOffsetFromCenter(2)
	//	, mMinBranch(2)
	//	, mMaxBranch(5)
	//  , mMirrorBranches(false)


	// Coniferous

	//, mMinHeight(32)
	//	, mMaxHeight(44)
	//	, mMinNodes(6)
	//	, mMaxNodes(10)
	//	, mMinDistBetweenNodes(5)
	//	, mMaxDistBetweenNodes(7)
	//	, mMaxOffsetFromCenter(0)
	//	, mMinBranch(20)
	//	, mMaxBranch(30)
	//	, mMirrorBranches(true)

TreeGenerator::TreeGenerator()
	: AssetGenerator()
	, mMinHeight(32)
	, mMaxHeight(44)
	, mMinNodes(6)
	, mMaxNodes(10)
	, mMinDistBetweenNodes(5)
	, mMaxDistBetweenNodes(7)
	, mMaxOffsetFromCenter(2)
	, mMinBranch(2)
	, mMaxBranch(5)
	, mMirrorBranches(false)
{
	//setup(Vector2(0, 0), Vector2(1024,1024), 32);
	setup();
	buildTree();
}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	AssetGenerator::setup(pos, size, numPixels);

	mDebugColor = new Color(1, 0, 0);
	mColorRamp.push_back(new Color(0.20, 0.20, 0.20));

	mTrunkColorRamp.push_back(new Color(0.275, 0.125, 0.125));
	mTrunkColorRamp.push_back(new Color(0.450, 0.175, 0.175));
	mTrunkColorRamp.push_back(new Color(0.600, 0.275, 0.155));

	if (!mMirrorBranches)
	{
		mLeafColorRamp.push_back(new Color(0.175, 0.410, 0.255));
		mLeafColorRamp.push_back(new Color(0.315, 0.610, 0.295));
		mLeafColorRamp.push_back(new Color(0.490, 0.805, 0.355));
	}
	else
	{
		mLeafColorRamp.push_back(new Color(0.100, 0.200, 0.120));
		mLeafColorRamp.push_back(new Color(0.175, 0.410, 0.255));
		mLeafColorRamp.push_back(new Color(0.250, 0.510, 0.295));
	}
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
	AssetGenerator::_draw();
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

	float maxWidth = mAssetSize/12.8; // width of base of trunk


	for (int i = 0; i < numNodes; i++)
	{
		//int offsetBetweenNodes = rand() % (mMaxDistBetweenNodes - mMinDistBetweenNodes) + mMinDistBetweenNodes;
		int offsetBetweenNodes = float(height) / numNodes;

		if (i == 0)
			thisHeight = 0;
		else
			thisHeight = lastHeight + offsetBetweenNodes;

		int thisWidth = floor(maxWidth - (float(i) / numNodes) * maxWidth);

		int offsetFromCenter = mMaxOffsetFromCenter == 0 ? mMaxOffsetFromCenter : rand() % (mMaxOffsetFromCenter - (-mMaxOffsetFromCenter)) + (-mMaxOffsetFromCenter);
		
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



	sprayPixel(Vector2(mAssetSize / 2 - 1, mAssetSize / 2 - 1), mAssetSize / 2, 0.1, mTrunkColorRamp[2], true);
	sprayPixel(Vector2(mAssetSize / 2 - 1, mAssetSize / 2 - 1), mAssetSize / 2, 0.1, mTrunkColorRamp[0], true);



	std::set<int> usedNodes;

	for (int i = 0; i <= numBranches; i++)
	{
		
		int buildOffNode = 0;
		
		while (buildOffNode == 0) 
		{
			// need to adjust this for coniferous trees todo
			if (usedNodes.size() >= mNodes.size() - 8)
			{
				goto endBranches;
			}

			int t;// = rand() % (mNodes.size() - 8) + 4;
			if (!mMirrorBranches)
			{
				t = rand() % (mNodes.size() - 8) + 4;
			}
			else 
			{
				t = rand() % (mNodes.size() - 6) + 4;
			}
			
			if (usedNodes.find(t) == usedNodes.end())
			{
				usedNodes.insert(t);
				buildOffNode = t;
			}
		}


		

		//int branchLength = rand() % 6 + 12 - std::min(buildOffNode % 2 * 2, 7);
		int branchLength;// = std::max((rand() % 10 + 9) - std::max(buildOffNode, 5), 4);

		Vector2 branchEnd;// = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * branchLength, static_cast<Vector2>(mNodes[buildOffNode]).y - (rand() % 5 + 3));
		Vector2 branchMid;// = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * (branchLength - 3), static_cast<Vector2>(mNodes[buildOffNode]).y - 2);

		if (!mMirrorBranches)
		{

			branchLength = std::max((rand() % 10 + 9) - std::max(buildOffNode, 5), 4);
			branchEnd = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * branchLength, static_cast<Vector2>(mNodes[buildOffNode]).y - (rand() % 5 + 3));
			branchMid = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * (branchLength - 3), static_cast<Vector2>(mNodes[buildOffNode]).y - 2);

		}
		else
		{

			branchLength = (mNodes.size() - (buildOffNode / 2 * 2)) * 0.7;
			
			branchEnd = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * branchLength, static_cast<Vector2>(mNodes[buildOffNode]).y + 1);
			branchMid = Vector2(static_cast<Vector2>(mNodes[buildOffNode]).x + (buildOffNode % 2 == 0 ? -1 : 1) * (branchLength - 3), static_cast<Vector2>(mNodes[buildOffNode]).y + 1);

		}
		
		setPixel(branchEnd, mTrunkColorRamp[1]);
		setPixel(branchMid, mTrunkColorRamp[1]);

		addLine(branchEnd, branchMid + Vector2((buildOffNode % 2 == 1 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), mTrunkColorRamp[2]);
		addLine(branchEnd, branchMid, mTrunkColorRamp[1]);
		addLine(branchEnd, branchMid + Vector2((buildOffNode % 2 == 0 ? 1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), mTrunkColorRamp[0]);

		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 0), mTrunkColorRamp[2]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 1), mTrunkColorRamp[0]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 2), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? 0 : -1), (buildOffNode % 2 == 0 ? -1 : 0)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 3), mTrunkColorRamp[0]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 0), mTrunkColorRamp[2]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 1), mTrunkColorRamp[0]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 2), mTrunkColorRamp[1]);
		addLine(branchMid, static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 3), mTrunkColorRamp[0]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 0), mTrunkColorRamp[2]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 1), mTrunkColorRamp[0]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 2), mTrunkColorRamp[1]);
		addLine(branchMid + Vector2((buildOffNode % 2 == 0 ? -1 : 0), (buildOffNode % 2 == 0 ? 0 : -1)), static_cast<Vector2>(mNodes[buildOffNode]) + Vector2(0, 3), mTrunkColorRamp[0]);
	
	
		mLeaves.append(branchEnd);

		if (mMirrorBranches)
		{

			mLeaves.append(branchMid);
			mLeaves.append(static_cast<Vector2>(mNodes[buildOffNode]));
		}
	
	
	
	}

endBranches:;



	//mLeaves.shuffle();
	



	for (int i = 0; i < mLeaves.size(); i++)
	{
		Vector2 currentLeaf = static_cast<Vector2>(mLeaves[i]);


		//int size = rand() % 5 + 4;
		//int size = std::max(std::min(rand() % int((63-currentLeaf.y) / 3) + 2, 8), 4);

		int size;// = int(54 - currentLeaf.y) / 5 + 4;

		if (!mMirrorBranches)
		{
			size = int(mAssetSize * 0.84 - currentLeaf.y) / 5 + mAssetSize / 16;
		}
		else
		{
			size = 2;
		}

		//addCircle(currentLeaf, 3, 12, debugColor2);
		sprayPixel(currentLeaf, floor(size * 2.0), 0.7, mLeafColorRamp[0]);
		sprayPixel(currentLeaf, floor(size * 1.5), 0.6, mLeafColorRamp[0]);
		sprayPixel(currentLeaf, floor(size * 1.0), 0.6, mLeafColorRamp[0]);

		sprayPixel(currentLeaf + Vector2(0, 0), floor(size * 2.0), 0.2, mLeafColorRamp[1], true);
		sprayPixel(currentLeaf + Vector2(-2, -2), floor(size * 1.7), 0.6, mLeafColorRamp[1]);

		sprayPixel(currentLeaf + Vector2(-4, -4), floor(size * 1.7), 0.4, mLeafColorRamp[2], true);
	}



	



	

	// Determine branch nodes, 2 per branch
	
	// Color Trunk and branches

	// Color Leaves
}

}