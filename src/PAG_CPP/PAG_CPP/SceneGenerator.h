#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include <vector>
#include <list>
#include <set>

#include "AssetGenerator.h"



namespace godot
{

	class SceneGenerator : public Node2D
	{
		GODOT_CLASS(SceneGenerator, Node2D)

	public:
		SceneGenerator(Vector2 screenSize = Vector2(1280, 720), int pixelSize = 4);
		~SceneGenerator();

		virtual void setup();

		// Godot Functions
		static void _register_methods();
		virtual void _init();
		void _ready();
		void _draw();


		void updateDistance();
		void drawPixel(Vector2 pos, Color* color);

		void drawLine(Vector2 origin, Vector2 dest, Color* color, int flag /*= 0*/);
		Array getLine(Vector2 origin, Vector2 dest);


		inline float getDistance() { return mDistance; }
		inline void setDistance(float d) { mDistance = d; }

	private:


		struct BiomeBlend
		{
			float grass = 1.0;
			float darkGrass = 0.0f;
			float sand = 0.0;
			float snow = 0.0f;
			float rocky = 0.0f;
		};


		struct Asset
		{
			int band;
			int bandPos;
			AssetGenerator* asset;
		};


		float mBandPos[12]; // the y pos on screen of layer boundaries

		float mBandScrollSpeed[12];
		float mBandCurPos[12]; // the current left side of a band

		std::vector<std::vector<std::vector<Color*>>> mBands;

		Vector2 mScreenSize;
		Vector2 mScreenSizePixel;

		int mPixelSize;

		std::vector<Asset> mAssets;



		float mDistance;

	};




}