#pragma once
#include "AssetGenerator.h"

namespace godot
{

	class TreeGenerator : public AssetGenerator
	{

		GODOT_CLASS(TreeGenerator, Node2D)

	public:
		TreeGenerator();
		~TreeGenerator();

		virtual void setup(Vector2 pos = Vector2(0, 0), Vector2 size = Vector2(1024, 1024), int numPixels = 64);

		// Godot Functions
		static void _register_methods();
		virtual void _init();
		void _draw();

		// Construct Tree
		void buildTree();

		void setType(int type);

	private:

		// Anchor points on trunk (in pairs eg. [0] = left point [1] = right point)
		Array mNodes;

		// Anchor points for leaves (leaves centered on these points)
		Array mLeaves;

		// Trees need multiple color ramps
		std::vector<Color*> mTrunkColorRamp;
		std::vector<Color*> mLeafColorRamp;

		// Tree Variables
		int mMinHeight;
		int mMaxHeight;

		int mMinNodes;
		int mMaxNodes;

		int mMinDistBetweenNodes;
		int mMaxDistBetweenNodes; 

		int mMaxOffsetFromCenter;

		int mMinBranch;
		int mMaxBranch;

		bool mMirrorBranches;
};

}

