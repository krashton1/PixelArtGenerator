#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include <vector>



namespace godot
{

	class ArtGenerator : public Node2D
	{
		GODOT_CLASS(ArtGenerator, Node2D)

	public:
		ArtGenerator();
		~ArtGenerator();
		static void _register_methods();

		void _init();
		void ready();
		void draw();

	private:

		void drawPixel(Vector2 pos, Color* color);
		void drawPixelArray();

		void setPixel(Vector2 pos, Color* color);

		void resetPixelArray();


		const Color BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);

		int mAssetSize;
		float mPixelSize;
		Vector2 mScreenSize;

		std::vector<std::vector<Color*>> mPixelArray;


	};




}