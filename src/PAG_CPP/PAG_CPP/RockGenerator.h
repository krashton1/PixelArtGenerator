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

		static void _register_methods();

		virtual void _init();
		void draw();

		void buildRock();


	private:

		int mMinPts;
		int mMaxPts;

		int mMinRadius;
		int mMaxRadius;

		//std::vector<Vector2> mPoints;

		//std::list<Vector2> mPoints;

		Array mPoints;


	};

}


