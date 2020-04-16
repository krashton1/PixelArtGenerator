#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include "AssetGenerator.h"
#include "RockGenerator.h"
#include "TreeGenerator.h"

#include "SceneGenerator.h"

#include <vector>

namespace godot
{
	class Main : public Node2D
	{
		GODOT_CLASS(Main, Node2D)

	public:
		Main();
		~Main();

		// Godot Functions
		static void _register_methods();
		void _init();
		void _ready();
		void _draw();
		void _process();

		void initSceneGenerator(Ref<ResourceInteractiveLoader> loader);

	private:

		std::vector<AssetGenerator*> mGenerators;
		SceneGenerator* mSceneGenerator;

		Node2D* mMapScene;

		int mTime;
		int mPan = 0;

		Ref<ResourceInteractiveLoader> mLoader;
		bool mIsLoading = false;

		Array mVisits;

		int mCurrentBiomeIndex = 0;

	};

}