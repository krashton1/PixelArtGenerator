#pragma once
#include "ArtGenerator.h"

namespace godot
{

	class RockGenerator : public ArtGenerator
	{
		GODOT_CLASS(RockGenerator, Node2D)

	public:
		RockGenerator();
		~RockGenerator();

		virtual void setup(Vector2 pos = Vector2(0, 0), Vector2 size = Vector2(1024, 1024), int numPixels = 64);

		// Godot Functions
		static void _register_methods();
		virtual void _init();
		void _draw();

		// Construct Rock
		void buildRock();

	private:

		// Points along rock edge where vertices form
		inline Array getPoints() { return mPoints; }

		int mMinPts;
		int mMaxPts;

		int mMinRadius;
		int mMaxRadius;

		// Vertices on rock edge
		Array mPoints;
	};

}


