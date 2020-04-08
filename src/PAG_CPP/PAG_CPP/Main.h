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

	private:

		std::vector<AssetGenerator*> mGenerators;
		SceneGenerator* mSceneGenerator;

		int mTime;
		bool mPan = false;

	};

}