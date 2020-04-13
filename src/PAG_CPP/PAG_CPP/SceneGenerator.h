#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>

#include "AssetGenerator.h"
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>

#include <Image.hpp>

#include <ImageTexture.hpp>


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


		void updateDistance(float deltaTime);
		void drawPixel(Vector2 pos, Color* color);

		void drawLine(Vector2 origin, Vector2 dest, Color* color, int flag /*= 0*/);
		Array getLine(Vector2 origin, Vector2 dest);
		Color* getAvgColor(Color* color1, Color* color2, float ratio);


		inline float getDistance() { return mDistance; }
		inline void setDistance(float d) { mDistance = d; }

	private:

		enum Biome
		{
			BiomeCloud = -2,
			BiomeMountain = -1,
			BiomeGrass = 0,
			BiomeBoreal = 1,
			BiomeSand = 2,
			BiomeSnow = 3,
			BiomeRock = 4

		};

		AssetGenerator* createAsset(Biome biomeType, bool forcePrimary = false);

		struct Asset
		{
			int band;
			int bandPos;
			int yShift = 0;
			AssetGenerator* asset;
			bool toUpdate = false;
			bool isCloud = false;
		};


		float mDistance;

		std::vector<std::vector<std::vector<Color*>>> mBands;

		float mBandInterupt[12]; // the y pos on screen of layer boundaries
		float mBandScrollSpeed[12];
		float mBandCurPos[12]; // the current left side of a band

		std::vector<std::deque<Ref<ImageTexture>>> mBandImages;
		int mBandImageCount[12];

		Vector2 mScreenSize;
		Vector2 mScreenSizePixel;
		int mPixelSize;


		int mNumAssets;
		std::vector<Asset> mAssets;

		Ref<PackedScene> mTreeGenScene;
		Ref<PackedScene> mRockGenScene;

		std::map<int, std::vector<Color*>> mGroundColors;

		Biome mInitBiome;
		Biome mDestBiome;
		Biome mBackgroundBiome;

		int mCurrentMountainHeight = 30;
		std::map<int, int> mBiomeMountainHeights;

	};




}