#pragma once
#include "AssetGenerator.h"

namespace godot
{

	class RockGenerator : public AssetGenerator
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

		// Vertices on rock edge
		Array mPoints;

		// Rock Variables
		int mMinPts;
		int mMaxPts;
		int mMinRadius;
		int mMaxRadius;
	};

}


