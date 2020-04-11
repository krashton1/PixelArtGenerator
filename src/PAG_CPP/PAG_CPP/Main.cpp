

#include "Main.h"
#include <windows.h>
#include <OS.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <cassert>  
#include <time.h>
#include <Input.hpp>

namespace godot
{

Main::Main()
{
	// Wait for Debugger
	if (OS::get_singleton()->get_cmdline_args().size() > 0 && OS::get_singleton()->get_cmdline_args()[0] == String("-waitForDebug"))
	{
		while (!IsDebuggerPresent())
			Sleep(100);
	}

	// Init rand
	srand(time(NULL));
}

Main::~Main()
{

}

void Main::_register_methods()
{
	register_method((char*)"_ready", &_ready);
	register_method((char*)"_draw", &_draw);
	register_method((char*)"_init", &_init);
	register_method((char*)"_process", &_process);
}

void Main::_init()
{
	Ref<PackedScene> artGenScene = ResourceLoader::get_singleton()->load("res://AssetGenerator.tscn");
	Ref<PackedScene> rockGenScene = ResourceLoader::get_singleton()->load("res://RockGenerator.tscn");
	Ref<PackedScene> treeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");





	// ---------------------------------------- Art ---------------------------------------- //

	AssetGenerator* smileGen = Object::cast_to<AssetGenerator>(artGenScene->instance());
	smileGen->addSmile();
	smileGen->set_position(Vector2(700, 0));
	smileGen->apply_scale(Vector2(0.2, 0.2));
	add_child(smileGen, "smileGen");
	mGenerators.push_back(smileGen);


	//// ---------------------------------------- Rocks ---------------------------------------- //

	//for (int i = 0; i < 6; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());
	//		rockGen->setType(RockGenerator::RockTypeBush);

	//		rockGen->apply_scale(Vector2(0.2, 0.2));
	//		rockGen->set_position(Vector2(i * 200, j * 200));
	//		add_child(rockGen, "rockGen" + i + j);
	//		mGenerators.push_back(rockGen);
	//	}
	//}


	// ---------------------------------------- Trees ---------------------------------------- //
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
			treeGen->setType(TreeGenerator::TreeTypeSnowConiferous);
			treeGen->apply_scale(Vector2(0.2, 0.2));
			treeGen->set_position(Vector2(i * 200, j * 200));
			add_child(treeGen, "treeGen" + i + j);
			mGenerators.push_back(treeGen);
		}
	}

	//Ref<PackedScene> sceneGenScene = ResourceLoader::get_singleton()->load("res://SceneGenerator.tscn");
	//SceneGenerator* sceneGen = Object::cast_to<SceneGenerator>(sceneGenScene->instance());
	//add_child(sceneGen, "sceneGen");
	//mSceneGenerator = sceneGen;

}

void Main::_ready()
{

}

void Main::_draw()
{

}

void Main::_process()
{
	float lastTime = mTime;
	float currentTime = float(clock());
	float deltaTime = (currentTime - lastTime) / 1000.0;
	mTime = currentTime;

	// Rebuild all rocks whenever 'right' is pressed
	if (Input::get_singleton()->is_action_pressed("ui_right"))
	{
		mPan = true;
	}
	if (Input::get_singleton()->is_action_pressed("ui_left"))
	{
		mPan = false;
	}

	if(mPan)
	{
		mSceneGenerator->updateDistance(deltaTime);
		mSceneGenerator->update();

	}
}

}