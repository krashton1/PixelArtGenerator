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

	//ArtGenerator* smileGen = Object::cast_to<ArtGenerator>(artGenScene->instance());
	//Color* black = new Color(0.0, 0.0, 0.0);

	//smileGen->set_position(Vector2(600, 100));

	//std::vector<std::vector<Color*>> smileArr;
	//smileArr.resize(5);
	//for (int x = 0; x < smileArr.size(); x++)
	//	smileArr[x].resize(5);

	//smileArr[1][1] = black;
	//smileArr[1][3] = black;
	//smileArr[3][0] = black;
	//smileArr[3][4] = black;
	//smileArr[4][1] = black;
	//smileArr[4][2] = black;
	//smileArr[4][3] = black;

	//for (int x = 0; x < smileArr.size(); x++)
	//{
	//	for (int y = 0; y < smileArr[x].size(); y++)
	//	{
	//		if (smileArr[x][y] != nullptr)
	//			smileGen->setPixel(Vector2(x, y), smileArr[x][y]);
	//	}
	//}

	//smileGen->pivotPixelArray();
	//add_child(smileGen, "smileGen");
	//mGenerators.push_back(smileGen);



	Ref<PackedScene> rockGenScene = ResourceLoader::get_singleton()->load("res://RockGenerator.tscn");

	//RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());
	////ArtGenerator* artGen2 = Object::cast_to<ArtGenerator>(artGenScene->instance());

	////RockGenerator* rockGen = static_cast<RockGenerator*>(Object::cast_to<ArtGenerator>(artGenScene->instance()));
	////rockGen->buildRock();
	//rockGen->apply_scale(Vector2(0.2, 0.2));
	//add_child(rockGen, "rockGen");
	//mGenerators.push_back(rockGen);
	//rockGen->set_position(Vector2(600, 0));

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			RockGenerator* rockGen = Object::cast_to<RockGenerator>(rockGenScene->instance());

			rockGen->apply_scale(Vector2(0.2, 0.2));
			add_child(rockGen, "rockGen" + i + j);
			mGenerators.push_back(rockGen);
			rockGen->set_position(Vector2(i * 200, j * 200));
		}
	}

	



	//ArtGenerator* sprayGen = Object::cast_to<ArtGenerator>(artGenScene->instance());
	//sprayGen->apply_scale(Vector2(0.5, 0.5));
	//add_child(sprayGen, "sprayGen");
	//mGenerators.push_back(sprayGen);

	//sprayGen->addLine(Vector2(0, 0), Vector2(0, 63), new Color(0, 0, 0));
	//sprayGen->addLine(Vector2(63, 0), Vector2(63, 63), new Color(0, 0, 0));
	//sprayGen->addLine(Vector2(0, 0), Vector2(63, 0), new Color(0, 0, 0));
	//sprayGen->addLine(Vector2(0, 63), Vector2(63, 63), new Color(0, 0, 0));

	//sprayGen->sprayPixel(Vector2(31, 31), 10.0, 0.2, new Color(0, 0, 1), true);

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