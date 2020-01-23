#include "Main.h"

#include <windows.h>
#include <OS.hpp>

#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

#include <cassert>

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


	



	

}

Main::~Main()
{

}

void Main::_register_methods()
{
	register_method((char*)"_ready", &ready);
	register_method((char*)"_draw", &draw);
	register_method((char*)"_init", &_init);
}

void Main::_init()
{

	//ResourceLoader *resource_loader = ResourceLoader::get_singleton();
	//Ref<Resource> res = ResourceLoader::load("res://ArtGenerator.tscn");
	//Ref<PackedScene> res = Object::cast_to<PackedScene>(resource_loader->load("res://ArtGenerator.tscn"));

	Ref<PackedScene> artGenScene = ResourceLoader::get_singleton()->load("res://ArtGenerator.tscn");
	assert(artGenScene.is_valid());

	//Node* node = scene->instance();

	ArtGenerator* artGen = Object::cast_to<ArtGenerator>(artGenScene->instance());
	//body->set_translation(get_transform().origin + position);
	// set direction variable defined other node object (c++ code variable) 
	
	//artGen->setPixel(Vector2(5, 0), new Color(1,0,0,1));
	artGen->set_position(Vector2(100, 100));

	//artGen->setup();


	Color* black = new Color(0.0, 0.0, 0.0);

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
				artGen->setPixel(Vector2(x, y), smileArr[x][y]);
		}
	}

	artGen->pivotPixelArray();

	add_child(artGen, "artGenerator");

}

void Main::ready()
{

}

void Main::draw()
{

}

}