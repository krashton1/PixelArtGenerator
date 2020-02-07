#pragma once
#include "ArtGenerator.h"

namespace godot
{


class TreeGenerator : public ArtGenerator
{

	GODOT_CLASS(TreeGenerator, Node2D)

public:
	TreeGenerator();
	~TreeGenerator();

	static void _register_methods();

	virtual void _init();
	void draw();
};

}

