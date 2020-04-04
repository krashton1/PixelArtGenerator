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


	// ---------------------------------------- Rocks ---------------------------------------- //

	for (int i = 0; i < 0; i++)
	{
		for (int j = 0; j < 0; j++)
		{
			RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());

			rockGen->apply_scale(Vector2(0.2, 0.2));
			rockGen->set_position(Vector2(i * 200, j * 200));
			add_child(rockGen, "rockGen" + i + j);
			mGenerators.push_back(rockGen);
		}
	}


	// ---------------------------------------- Trees ---------------------------------------- //
	for (int i = 0; 0 < 0; i++)
	{
		for (int j = 0; j < 0; j++)
		{
			TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
			treeGen->apply_scale(Vector2(0.2, 0.2));
			treeGen->set_position(Vector2(i * 200, j * 200));
			add_child(treeGen, "treeGen" + i + j);
			mGenerators.push_back(treeGen);
		}
	}











	Ref<PackedScene> sceneGenScene = ResourceLoader::get_singleton()->load("res://SceneGenerator.tscn");

	SceneGenerator* sceneGen = Object::cast_to<SceneGenerator>(sceneGenScene->instance());
	add_child(sceneGen, "sceneGen");
	mSceneGenerator = sceneGen;





}

void Main::_ready()
{

}

void Main::_draw()
{

}

void Main::_process()
{
	// Rebuild all rocks whenever 'right' is pressed
	if (Input::get_singleton()->is_action_just_pressed("ui_right"))
	{
		//for (AssetGenerator* artGen : mGenerators)
		//{
		//	if (RockGenerator* rockGen = dynamic_cast<RockGenerator*>(artGen))
		//	{
		//		rockGen->buildRock();
		//		rockGen->update();
		//	}

		//	if (TreeGenerator* treeGen = dynamic_cast<TreeGenerator*>(artGen))
		//	{
		//		treeGen->buildTree();
		//		treeGen->update();
		//	}
		//}


		mSceneGenerator->setDistance(mSceneGenerator->getDistance() + 0.01);
		mSceneGenerator->updateDistance();
		mSceneGenerator->update();

	}
}

}