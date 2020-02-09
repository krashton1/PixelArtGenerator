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

		virtual void setup(Vector2 pos = Vector2(0, 0), Vector2 size = Vector2(1024, 1024), int numPixels = 64);

		// Godot Functions
		static void _register_methods();
		virtual void _init();
		void _draw();

		// Construct Tree
		void buildTree();

	private:
};

}

