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

	srand(time(NULL));
}

Main::~Main()
{

}

void Main::_register_methods()
{
	register_method((char*)"_ready", &ready);
	register_method((char*)"_draw", &draw);
	register_method((char*)"_init", &_init);
	register_method((char*)"_process", &process);
	
}

void Main::_init()
{
	Ref<PackedScene> artGenScene = ResourceLoader::get_singleton()->load("res://ArtGenerator.tscn");

	ArtGenerator* smileGen = Object::cast_to<ArtGenerator>(artGenScene->instance());
	Color* black = new Color(0.0, 0.0, 0.0);

	smileGen->set_position(Vector2(500, 100));

	std::vector<std::vector<Color*>> smileArr;
	smileArr.resize(5);
	for (int x = 0; x < smileArr.size(); x++)
		smileArr[x].resize(5);

	smileArr[1][1] = black;
	smileArr[1][3] = black;
	smileArr[3][0] = black;
	smileArr[3][4] = black;
	smileArr[4][1] = black;
	smileArr[4][2] = black;
	smileArr[4][3] = black;

	for (int x = 0; x < smileArr.size(); x++)
	{
		for (int y = 0; y < smileArr[x].size(); y++)
		{
			if (smileArr[x][y] != nullptr)
				smileGen->setPixel(Vector2(x, y), smileArr[x][y]);
		}
	}

	smileGen->pivotPixelArray();
	add_child(smileGen, "smileGen");
	mGenerators.push_back(smileGen);



	Ref<PackedScene> rockGenScene = ResourceLoader::get_singleton()->load("res://RockGenerator.tscn");

	RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());
	//ArtGenerator* artGen2 = Object::cast_to<ArtGenerator>(artGenScene->instance());

	//RockGenerator* rockGen = static_cast<RockGenerator*>(Object::cast_to<ArtGenerator>(artGenScene->instance()));
	//rockGen->buildRock();
	rockGen->apply_scale(Vector2(0.5, 0.5));
	add_child(rockGen, "rockGen");
	mGenerators.push_back(rockGen);

}

void Main::ready()
{

}

void Main::draw()
{

}

void Main::process()
{
	if (Input::get_singleton()->is_action_just_pressed("ui_right"))
	{
		int i = 0;
		RockGenerator* rockGen = dynamic_cast<RockGenerator*>(mGenerators[1]);
		rockGen->buildRock();
		rockGen->update();
		return;
	}
}

}