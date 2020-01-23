#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
//#include <Input.hpp>

#include "ArtGenerator.h"

#include <vector>

namespace godot
{
	class Main : public Node2D
	{
		GODOT_CLASS(Main, Node2D)

	public:
		Main();
		~Main();

		static void _register_methods();
		void _init();
		void ready();
		void draw();

	private:

		std::vector<ArtGenerator*> mGenerators;

	};

}