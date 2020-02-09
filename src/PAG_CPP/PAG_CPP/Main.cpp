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
	Ref<PackedScene> artGenScene = ResourceLoader::get_singleton()->load("res://ArtGenerator.tscn");
	Ref<PackedScene> rockGenScene = ResourceLoader::get_singleton()->load("res://RockGenerator.tscn");
	Ref<PackedScene> treeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");


	// ---------------------------------------- Art ---------------------------------------- //

	ArtGenerator* smileGen = Object::cast_to<ArtGenerator>(artGenScene->instance());
	smileGen->addSmile();
	smileGen->set_position(Vector2(600, 100));
	add_child(smileGen, "smileGen");
	mGenerators.push_back(smileGen);


	// ---------------------------------------- Rocks ---------------------------------------- //

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());

			rockGen->apply_scale(Vector2(0.2, 0.2));
			rockGen->set_position(Vector2(i * 200, j * 200));
			add_child(rockGen, "rockGen" + i + j);
			mGenerators.push_back(rockGen);
		}
	}


	// ---------------------------------------- Trees ---------------------------------------- //

	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
	treeGen->apply_scale(Vector2(0.5, 0.5));
	treeGen->set_position(Vector2(320, 0));
	add_child(treeGen, "treeGen");
	mGenerators.push_back(treeGen);



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
		for (ArtGenerator* artGen : mGenerators)
		{
			if (RockGenerator* rockGen = dynamic_cast<RockGenerator*>(artGen))
			{
				rockGen->buildRock();
				rockGen->update();
			}
		}
	}
}

}