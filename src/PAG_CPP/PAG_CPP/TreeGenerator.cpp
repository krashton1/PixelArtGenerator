#include "TreeGenerator.h"


namespace godot
{


TreeGenerator::TreeGenerator()
	: ArtGenerator()
{
	setup();
	buildTree();
}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::setup(Vector2 pos /*= Vector2(0, 0)*/, Vector2 size /*= Vector2(1024, 1024)*/, int numPixels /*= 64*/)
{
	ArtGenerator::setup(pos, size, numPixels);
}

void TreeGenerator::_register_methods()
{
	register_method((char*)"_draw", &_draw);
}

void TreeGenerator::_init()
{
	// Do nothing
}

void TreeGenerator::_draw()
{
	ArtGenerator::_draw();
}

void TreeGenerator::buildTree()
{
	addSmile();
}

}