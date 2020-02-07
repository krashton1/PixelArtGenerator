#include "TreeGenerator.h"


namespace godot
{


TreeGenerator::TreeGenerator()
	: ArtGenerator()
{
	setup();

	mPixelArray[0][0] = new Color(0, 0, 0);
}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::_register_methods()
{
	register_method((char*)"_draw", &draw);

}

void TreeGenerator::_init()
{

}

void TreeGenerator::draw()
{
	ArtGenerator::draw();

}

}