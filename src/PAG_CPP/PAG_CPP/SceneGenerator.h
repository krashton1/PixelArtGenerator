#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include <vector>
#include <list>
#include <set>



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

		struct mBiomeBlend
		{
			float grass = 1.0;
			float darkGrass = 0.0f;
			float sand = 0.0;
			float snow = 0.0f;
			float rocky = 0.0f;
		};

		struct mLayerScrollSpeed
		{
			float layer0 = 1.0; //foreground
			float layer1 = 0.9;
			float layer2 = 0.8;
			float layer3 = 0.7;
			float layer4 = 0.6;
			float layer5 = 0.5; 
			float layer6 = 0.4; 
			float layer7 = 0.3;
			float layer8 = 0.2; 
			float layer9 = 0.1; //background
			float layer10 = 0.0; //sky

		};

		std::vector<std::vector<std::vector<Color*>>> mLayerGrounds;

		Vector2 mScreenSize;
		Vector2 mScreenSizePixel;
		int mPixelSize;



		float mDistance;

	};




}