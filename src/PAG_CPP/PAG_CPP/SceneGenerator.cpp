#include "SceneGenerator.h"
#include "AssetGenerator.h"
#include "TreeGenerator.h"
#include "RockGenerator.h"

#include <cmath>
#include <algorithm> 

#include <string>
#include <functional>
#include <optional>


namespace godot
{

	SceneGenerator::SceneGenerator(Vector2 screenSize /*= Vector2(1024, 1024)*/, int pixelSize /*= 16*/)
		: mScreenSize(screenSize)
		, mPixelSize(pixelSize)
		, mDistance(0.0f)
	{
		// Screen size in pixels
		mScreenSizePixel = Vector2(mScreenSize.x / mPixelSize, mScreenSize.y / mPixelSize);

		// The y position between bands
		mBandInterupt[0] = (mScreenSizePixel.y / 10.0f * 2.50f * mPixelSize) - mPixelSize;
		mBandInterupt[1] = (mScreenSizePixel.y / 10.0f * 2.50f * mPixelSize) - mPixelSize;
		mBandInterupt[2] = (mScreenSizePixel.y / 10.0f * 2.75f * mPixelSize) - mPixelSize;
		mBandInterupt[3] = (mScreenSizePixel.y / 10.0f * 3.00f * mPixelSize) - mPixelSize;
		mBandInterupt[4] = (mScreenSizePixel.y / 10.0f * 3.25f * mPixelSize) - mPixelSize;
		mBandInterupt[5] = (mScreenSizePixel.y / 10.0f * 3.60f * mPixelSize) - mPixelSize;
		mBandInterupt[6] = (mScreenSizePixel.y / 10.0f * 4.10f * mPixelSize) - mPixelSize;
		mBandInterupt[7] = (mScreenSizePixel.y / 10.0f * 4.80f * mPixelSize) - mPixelSize;
		mBandInterupt[8] = (mScreenSizePixel.y / 10.0f * 5.70f * mPixelSize) - mPixelSize;
		mBandInterupt[9] = (mScreenSizePixel.y / 10.0f * 6.80f * mPixelSize) - mPixelSize;
		mBandInterupt[10] = (mScreenSizePixel.y / 10.0f * 8.10f * mPixelSize) - mPixelSize;
		mBandInterupt[11] = (mScreenSizePixel.y / 10.0f * 10.0f * mPixelSize);


		// Scroll speed of each band in order to create parallax effect
		mBandScrollSpeed[0] = 1.50f;
		mBandScrollSpeed[1] = 2.0f;
		mBandScrollSpeed[2] = 3.0f;
		mBandScrollSpeed[3] = 3.3f;
		mBandScrollSpeed[4] = 3.6f;
		mBandScrollSpeed[5] = 4.0f;
		mBandScrollSpeed[6] = 4.6f;
		mBandScrollSpeed[7] = 5.6f;
		mBandScrollSpeed[8] = 6.8f;
		mBandScrollSpeed[9] = 8.4f;
		mBandScrollSpeed[10] = 10.0f;
		mBandScrollSpeed[11] = 12.8f;

		for (int i = 0; i < 12; i++)
		{
			mBandInterupt[i] = mBandInterupt[i] - (fmod(mBandInterupt[i], mPixelSize));
			mBandCurPos[i] = 0.0f;
			mBandImageCount[i] = 1;
		}

		// Ground colors
		mGroundColors.insert({ BiomeCloud, std::vector<Color*> { new Color(.075, 0.705, 0.940), new Color(.075, 0.685, 0.910) } });
		mGroundColors.insert({ BiomeMountain, std::vector<Color*> { new Color(0.4, 0.4, 0.5), new Color(0.5, 0.5, 0.6) } });
		mGroundColors.insert({ BiomeGrass, std::vector<Color*> { new Color(0.2, 0.6, 0.2), new Color(0.2, 0.5, 0.2) } });
		mGroundColors.insert({ BiomeBoreal, std::vector<Color*> { new Color(0.1, 0.2, 0.1), new Color(0.2, 0.3, 0.1) } });
		mGroundColors.insert({ BiomeSand, std::vector<Color*> { new Color(0.8, 0.7, 0.6), new Color(0.95, 0.85, 0.75) } }); 
		mGroundColors.insert({ BiomeSnow, std::vector<Color*> { new Color(0.95, 0.95, 1), new Color(0.85, 0.85, 0.95), new Color(0.1, 0.2, 0.1) } });
		mGroundColors.insert({ BiomeRock, std::vector<Color*> { new Color(0.4, 0.4, 0.4), new Color(0.5, 0.5, 0.5) } });


		mBiomeMountainHeights.insert({ BiomeCloud, 1 });
		mBiomeMountainHeights.insert({ BiomeMountain, 20 });
		mBiomeMountainHeights.insert({ BiomeGrass, 5 });
		mBiomeMountainHeights.insert({ BiomeBoreal, 5 });
		mBiomeMountainHeights.insert({ BiomeSand, 5 });
		mBiomeMountainHeights.insert({ BiomeSnow, 5 });
		mBiomeMountainHeights.insert({ BiomeRock, 20 });

		mInitBiome = BiomeRock;
		mDestBiome = BiomeSnow;

		mBackgroundBiome = mInitBiome;

		setup();
	}

	SceneGenerator::~SceneGenerator()
	{

	}

	void SceneGenerator::_register_methods()
	{
		register_method((char*)"_ready", &_ready);
		register_method((char*)"_draw", &_draw);
	}

	void SceneGenerator::_init()
	{
		// Load the Generators
		mTreeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");
		mRockGenScene = ResourceLoader::get_singleton()->load("res://RockGenerator.tscn");

		float scalings[12] = { 0.2,0,0.03,0.05,0.05,0.08,0.12,0.17,0.23,0.30,0.38,0.45 };

		RockGenerator* assetGen;
		Asset newAsset;

		// Populate the layers
		for (int i = 2; i < 7; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				AssetGenerator* assetGen = createAsset(mInitBiome, (i > 8 ? true : false));
				assetGen->apply_scale(Vector2(scalings[i], scalings[i]));

				newAsset.band = i;
				newAsset.bandPos = rand() % 360 - 20;
				newAsset.asset = assetGen;

				assetGen->set_z_index(20 + i);

				add_child(assetGen);
				mAssets.push_back(newAsset);
				mNumAssets++;
			}
		}

		for (int i = 7; i < 10; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				AssetGenerator* assetGen = createAsset(mInitBiome, (i > 8 ? true : false));
				assetGen->apply_scale(Vector2(scalings[i], scalings[i]));

				newAsset.band = i;
				newAsset.bandPos = rand() % 360 - 20;
				newAsset.asset = assetGen;

				assetGen->set_z_index(20 + i);
				add_child(assetGen);
				mAssets.push_back(newAsset);
				mNumAssets++;
			}
		}

		for (int i = 10; i < 12; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				//assetGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
				AssetGenerator* assetGen = createAsset(mInitBiome, (i > 8 ? true : false));
				assetGen->apply_scale(Vector2(scalings[i], scalings[i]));

				newAsset.band = i;
				newAsset.bandPos = rand() % 360 - 20;
				newAsset.asset = assetGen;

				assetGen->set_z_index(20 + i);
				add_child(assetGen);
				mAssets.push_back(newAsset);
				mNumAssets++;
			}
		}



		int numMountains = 20;
		for (int j = 0; j < numMountains; j++)
		{
			//assetGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
			assetGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			assetGen->apply_scale(Vector2(.175, .175));
			assetGen->addMountain(mCurrentMountainHeight * 1.2, mGroundColors[(mCurrentMountainHeight >= 15 ? BiomeMountain : mInitBiome)][0], mGroundColors[(mCurrentMountainHeight >= 15 ? BiomeMountain : mInitBiome)][1]);

			newAsset.band = 0;
			newAsset.bandPos = (mScreenSizePixel.x + 80) / numMountains * j + (mScreenSizePixel.x + 80) / (2 * numMountains) - 40;
			newAsset.asset = assetGen;
			newAsset.yShift = 34;

			assetGen->set_z_index(rand() % 10 + 1 );

			add_child(assetGen);
			mAssets.push_back(newAsset);
			mNumAssets++;
		}

		// Clouds
		for (int j = 0; j < 6; j++)
		{
			//assetGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
			assetGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			assetGen->apply_scale(Vector2(scalings[0], scalings[0]));
			assetGen->setType(RockGenerator::RockTypeCloud);

			newAsset.band = 0;
			newAsset.bandPos = rand() % 360 - 20;
			newAsset.asset = assetGen;
			newAsset.yShift = 0;
			newAsset.isCloud = true;


			assetGen->set_z_index(11);

			add_child(assetGen);
			mAssets.push_back(newAsset);
			mNumAssets++;
		}

		for (int j = 0; j < numMountains; j++)
		{
			//assetGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
			assetGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			assetGen->apply_scale(Vector2(.175, .175));
			assetGen->addMountain(mCurrentMountainHeight * 1.2, mGroundColors[(mCurrentMountainHeight >= 15 ? BiomeMountain : mInitBiome)][0], mGroundColors[(mCurrentMountainHeight >= 15 ? BiomeMountain : mInitBiome)][1]);

			newAsset.band = 1;
			newAsset.bandPos = (mScreenSizePixel.x + 80) / numMountains * j - 40;
			newAsset.asset = assetGen;
			newAsset.yShift = 0;

			newAsset.isCloud = false;

			assetGen->set_z_index(rand() % 10 + 11);

			add_child(assetGen);
			mAssets.push_back(newAsset);
			mNumAssets++;
		}


		for (int j = 0; j < 6; j++)
		{
			//assetGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
			assetGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			assetGen->apply_scale(Vector2(scalings[0], scalings[0]));
			assetGen->setType(RockGenerator::RockTypeCloud);

			newAsset.band = 1;
			newAsset.bandPos = rand() % 360 - 20;
			newAsset.asset = assetGen;
			newAsset.yShift = -25;
			newAsset.isCloud = true;


			assetGen->set_z_index(rand() % 10 + 11);

			add_child(assetGen);
			mAssets.push_back(newAsset);
			mNumAssets++;
		}

	}

	void SceneGenerator::setup()
	{
		// Create sky tex
		std::vector<std::vector<Color*>> skyTex;
		for (int x = 0; x < mScreenSizePixel.x; x++)
		{
			std::vector<Color*> tempCol;
			for (int y = 0; y < mBandInterupt[0] / mPixelSize; y++)
			{
				tempCol.push_back(mGroundColors[BiomeCloud][rand() % 2]);
			}
			skyTex.push_back(tempCol);
		}
		mBands.push_back(skyTex);


		// Create background tex (this will be empty, bc sky tex meets ground tex. But background objects will be held in this band instead of sky band, since they scroll at a diff speed
		std::vector<std::vector<Color*>> backgroundTex;
		mBands.push_back(backgroundTex);


		// Create ground base textures
		for (int i = 2; i < 12; i++)
		{
			std::vector<std::vector<Color*>> bandTex;
			for (int x = 0; x < mScreenSizePixel.x; x++)
			{
				std::vector<Color*> tempCol;
				for (int y = mBandInterupt[i - 1]; y < mBandInterupt[i]; y += mPixelSize)
				{
					tempCol.push_back(mGroundColors[mInitBiome][rand() % 2]);
				}
				bandTex.push_back(tempCol);
			}
			mBands.push_back(bandTex);

		}

		// Create blended ground sprites
		for (int i = 0; i < 12; i++)
		{
			Ref<Image> bandImage;
			bandImage.instance();
			bandImage->create(mScreenSizePixel.x + 1, (mBandInterupt[i] - mBandInterupt[i - 1]) / mPixelSize + 1, false, Image::Format::FORMAT_RGBA8);
			bandImage->lock();

			int stupidVariableForStupidReasonX = mBands[i].size(); // if (-1 < mBands[i].size()) evals to false for some stupid reason. This is not a bug that should exist in 2020, but it do.
			for (int x = -1; x < stupidVariableForStupidReasonX; x++)
			{
				int stupidVariableForStupidReasonY = (i != 1 ? mBands[i][(x != -1 ? x : 0)].size() : -1); // if (-1 < mBands[i][x].size()) evals to false for some stupid reason. This is not a bug that should exist in 2020, but it do.
				for (int y = -1; y < stupidVariableForStupidReasonY; y++)
				{
					float avgR = 1.0;
					float avgG = 0.0;
					float avgB = 0.0;

					if (x == -1)
					{
						if (y != -1)
						{
							avgR = (mBands[i][x + 1][y]->r);
							avgG = (mBands[i][x + 1][y]->g);
							avgB = (mBands[i][x + 1][y]->b);
						}

					}
					else if (y == -1)
					{
						avgR = (mBands[i][x][y + 1]->r);
						avgG = (mBands[i][x][y + 1]->g);
						avgB = (mBands[i][x][y + 1]->b);

					}
					else if (y == 0)
					{

						avgR = (mBands[i][x][y]->r + mBands[i][x][y + 1]->r + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->r + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->r) / 4.0;
						avgG = (mBands[i][x][y]->g + mBands[i][x][y + 1]->g + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->g + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->g) / 4.0;
						avgB = (mBands[i][x][y]->b + mBands[i][x][y + 1]->b + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->b + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->b) / 4.0;
					}

					else if (y == mBands[i][x].size() - 1)
					{
						avgR = (mBands[i][x][y]->r + mBands[i][x][y - 1]->r + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->r + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->r) / 4.0;
						avgG = (mBands[i][x][y]->g + mBands[i][x][y - 1]->g + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->g + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->g) / 4.0;
						avgB = (mBands[i][x][y]->b + mBands[i][x][y - 1]->b + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->b + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->b) / 4.0;

					}
					else
					{
						avgR = (mBands[i][x][y]->r + mBands[i][x][y - 1]->r + mBands[i][x][y + 1]->r + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->r + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->r) / 5.0;
						avgG = (mBands[i][x][y]->g + mBands[i][x][y - 1]->g + mBands[i][x][y + 1]->g + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->g + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->g) / 5.0;
						avgB = (mBands[i][x][y]->b + mBands[i][x][y - 1]->b + mBands[i][x][y + 1]->b + mBands[i][((x - 1) < 0 ? int(mScreenSizePixel.x) - 1 : x - 1)][y]->b + mBands[i][((x + 1) % int(mScreenSizePixel.x))][y]->b) / 5.0;
					}

					float a = 1.0;

					if (x == -1)
						a -= 0.5;
					if (y == -1)
						a -= 0.5;
					//if (i == 2 && y == -1)
					//	a = 0.0;

					bandImage->set_pixel(x + 1, y + 1, Color(avgR, avgG, avgB, a));
				}
			}

			bandImage->unlock();
			//bandImage->save_png(("res://Assets/BandImages/bandImage" + std::to_string(i) + ".png").c_str());

			Ref<ImageTexture> tex;
			tex.instance();
			tex->create_from_image(bandImage, 0);

			std::deque<Ref<ImageTexture>> tempVec;
			tempVec.push_back(tex);
			mBandImages.push_back(tempVec);
			mBandImageCount[i]++;
		}
	}

	void SceneGenerator::_ready()
	{
		// Do nothing
	}

	void SceneGenerator::_draw()
	{
		// Draw bands from each layer (so objects from each layer overlap)
		for (int i = 0; i < 12; i++)
		{
			if (i == 1)
				continue;

			int toDelete = -1;

			for (int x = 0; x < mBandImages[i].size(); x++)
			{
				int prevX = (mBandImages[i][x]->get_width() == mScreenSizePixel.x + 1 ? 0 : (mScreenSizePixel.x * mPixelSize) + (mBandImageCount[i] - mBandImages[i].size() + x - 2) * mBandImages[i][x]->get_width() * mPixelSize);

				//for (int j = 0; j < x; j++)
				//	prevX += mBandImages[i][j]->get_width() * mPixelSize;

				int newX = -(mBandCurPos[i] * mPixelSize) + prevX;

				Rect2 rect = Rect2(newX - mPixelSize, (i == 0 ? 0 : mBandInterupt[i - 1]) - mPixelSize, (mBandImages[i][x]->get_width() * mPixelSize) + mPixelSize * (i == 0 ? 4 : 1), mBandInterupt[i] - (i == 0 ? 0 : mBandInterupt[i - 1]) + mPixelSize);

				if (rect.get_position().x + rect.get_size().x > 0)
					draw_texture_rect(mBandImages[i][x], rect, false);
				else
					toDelete = x;
			}

			if (toDelete != -1)
				mBandImages[i].pop_front();
		}

		// Draw assets in each layer
		for (int i = 0; i < mNumAssets; i++)
		{
			int x = (mAssets[i].bandPos - mBandCurPos[mAssets[i].band]) * mPixelSize - (mAssets[i].asset->get_scale().x * 1024 / 2.0);
			while (x < mAssets[i].asset->get_scale().x * -1024)
				x += mScreenSizePixel.x * mPixelSize + mAssets[i].asset->get_scale().x * 1024 * 1.5;

			int y;
			if (mAssets[i].band != 0)
				y = (mBandInterupt[mAssets[i].band] + mBandInterupt[mAssets[i].band - 1]) / 2 - (mAssets[i].asset->get_scale().y * 1024);
			else
				y = mBandInterupt[mAssets[i].band + 1] / 2 - (mAssets[i].asset->get_scale().y * 1024) * 0.7;
			y += mAssets[i].yShift;

			mAssets[i].asset->set_position(Vector2(x, y));
		}
	}

	void SceneGenerator::updateDistance(float deltaTime)
	{
		mDistance = mDistance + 0.02 * deltaTime;

		float oldBandPos[12];
		for (int i = 0; i < 12; i++)
		{
			oldBandPos[i] = mBandCurPos[i];
			mBandCurPos[i] = (mDistance * mScreenSizePixel.x) * mBandScrollSpeed[i];
		}


		if (((1 / mBandScrollSpeed[2]) * (mBandCurPos[2] / mScreenSizePixel.x)) - (mBandScrollSpeed[2] * 1.5 / 100.0) > 0.5)
			mBackgroundBiome = mDestBiome;

		//float initBiomeDensity = 1.0 - mDistance;
		//float initDestDensity = mDistance;

		for (int i = 0; i < 12; i++)
		{
			//float transitionChance = mBiomeBlend.darkGrass * (mBandScrollSpeed[11] / mBandScrollSpeed[i]);
			float transitionChance = ((1 / mBandScrollSpeed[i]) * (mBandCurPos[i] / mScreenSizePixel.x)) - (mBandScrollSpeed[i] * 1.5 / 100.0);

			// Calculate the base colors of each pixel
			if (mBandCurPos[i] > (mBandScrollSpeed[i] * ((mBandImageCount[i] - 1) * 4)) - i * 5) // todo turn on distributed loading for optimization "
			{
				if (i == 1)
					continue;

				std::vector<std::vector<Color*>> newBand;
				for (int x = 0; x < int(mBandScrollSpeed[i] * 4); x++)
				{
					std::vector<Color* > newVec;
					for (int y = 0; y < (i != 1 ? mBands[i][(x != -1 ? x : 0)].size() : -1); y++)
					{
						if (float(rand()) / float(RAND_MAX) < transitionChance)
							newVec.push_back(mGroundColors[(i == 0 ? BiomeCloud : mDestBiome)][rand() % mGroundColors[(i == 0 ? BiomeCloud : mDestBiome)].size()]);
						else
							newVec.push_back(mGroundColors[(i == 0 ? BiomeCloud : mInitBiome)][rand() % mGroundColors[(i == 0 ? BiomeCloud : mInitBiome)].size()]);
					}
					newBand.push_back(newVec);
				}

				// Calculate the blended texture
				Ref<Image> newImage;
				newImage.instance();
				newImage->create(int(mBandScrollSpeed[i] * 4 + 1), (mBandInterupt[i] - mBandInterupt[i - 1]) / mPixelSize + 1, false, Image::Format::FORMAT_RGBA8);
				newImage->lock();

				int stupidVariableForStupidReasonX = int(mBandScrollSpeed[i] * 4); // if (-1 < mBands[i].size()) evals to false for some stupid reason. This is not a bug that should exist in 2020, but it do.
				for (int x = -1; x < stupidVariableForStupidReasonX; x++)
				{
					int stupidVariableForStupidReasonY = (x != -1 ? newBand[x].size() : -1); // if (-1 < mBands[i][x].size()) evals to false for some stupid reason. This is not a bug that should exist in 2020, but it do.
					for (int y = -1; y < stupidVariableForStupidReasonY; y++)
					{
						float avgR = 1.0;
						float avgG = 0.0;
						float avgB = 0.0;

						if (y == -1)
						{
							if (x != -1)
							{
								avgR = (newBand[x][y + 1]->r);
								avgG = (newBand[x][y + 1]->g);
								avgB = (newBand[x][y + 1]->b);

							}
						}
						else if (x == -1)
						{
							avgR = (newBand[x + 1][y]->r);
							avgG = (newBand[x + 1][y]->g);
							avgB = (newBand[x + 1][y]->b);

						}
						else if (y == 0)
						{

							avgR = (newBand[x][y]->r + newBand[x][y + 1]->r + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->r + newBand[((x + 1) % newBand.size())][y]->r) / 4.0;
							avgG = (newBand[x][y]->g + newBand[x][y + 1]->g + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->g + newBand[((x + 1) % newBand.size())][y]->g) / 4.0;
							avgB = (newBand[x][y]->b + newBand[x][y + 1]->b + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->b + newBand[((x + 1) % newBand.size())][y]->b) / 4.0;
						}
						else if (y == newBand[x].size() - 1)
						{
							avgR = (newBand[x][y]->r + newBand[x][y - 1]->r + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->r + newBand[((x + 1) % newBand.size())][y]->r) / 4.0;
							avgG = (newBand[x][y]->g + newBand[x][y - 1]->g + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->g + newBand[((x + 1) % newBand.size())][y]->g) / 4.0;
							avgB = (newBand[x][y]->b + newBand[x][y - 1]->b + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->b + newBand[((x + 1) % newBand.size())][y]->b) / 4.0;

						}
						else
						{
							avgR = (newBand[x][y]->r + newBand[x][y - 1]->r + newBand[x][y + 1]->r + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->r + newBand[((x + 1) % newBand.size())][y]->r) / 5.0;
							avgG = (newBand[x][y]->g + newBand[x][y - 1]->g + newBand[x][y + 1]->g + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->g + newBand[((x + 1) % newBand.size())][y]->g) / 5.0;
							avgB = (newBand[x][y]->b + newBand[x][y - 1]->b + newBand[x][y + 1]->b + newBand[((x - 1) < 0 ? newBand.size() - 1 : x - 1)][y]->b + newBand[((x + 1) % newBand.size())][y]->b) / 5.0;
						}

						float a = 1.0;

						if (x == -1)
							a -= 0.5;
						if (y == -1)
							a -= 0.5;
						//if (i == 2 && y == -1)
						//	a = 0.0;

						newImage->set_pixel(x + 1, y + 1, Color(avgR, avgG, avgB, a));
						//newImage->set_pixel(x, y, *newBand[x][y]);
					}
				}

				newImage->unlock();
				//newImage->save_png(("res://Assets/BandImages/bandImage" + std::to_string(i) + "_"+ std::to_string(mBandImages[i].size()) + ".png").c_str());

				Ref<ImageTexture> newTex;
				newTex.instance();
				newTex->create_from_image(newImage, 0);

				mBandImages[i].push_back(newTex);
				mBandImageCount[i]++;
			}
		}

		// Refresh any assets that are ready to be refreshed
		for (int i = 0; i < mNumAssets; i++)
		{
			int x = mAssets[i].asset->get_position().x;

			if (mAssets[i].toUpdate && x > mScreenSize.x)
			{
				mAssets[i].toUpdate = false;
				mAssets[i].asset->queue_free();

				bool isTreeType = false;
				bool isInitBiome = true;


				float r = float(rand()) / float(RAND_MAX);
				//float transitionChance = mBiomeBlend.darkGrass * (mBandScrollSpeed[11] / mBandScrollSpeed[i]);
				float transitionChance = ((1 / mBandScrollSpeed[mAssets[i].band]) * (mBandCurPos[mAssets[i].band] / mScreenSizePixel.x)) - (mBandScrollSpeed[mAssets[i].band] * 1.5 / 100.0);

				if (r < transitionChance)
					isInitBiome = false;

				Biome biomeType = (isInitBiome ? mInitBiome : mDestBiome);
				if (mAssets[i].isCloud)
					biomeType = BiomeCloud;
				else if (mAssets[i].band <= 1)
					biomeType = BiomeMountain;

				AssetGenerator* assetGen = createAsset(biomeType, (mAssets[i].band > 8 ? true : false));

				assetGen->apply_scale(mAssets[i].asset->get_scale());
				assetGen->set_z_index(mAssets[i].band);

				assetGen->set_z_index(mAssets[i].band * 10 + rand() % 10);

				add_child(assetGen);
				mAssets[i].asset = assetGen;
			}

			if (!mAssets[i].toUpdate && x < -mAssets[i].asset->get_scale().x * 1024 / 2.0)
			{
				mAssets[i].toUpdate = true;
			}
		}
	}

	void SceneGenerator::drawPixel(Vector2 pos, Color* color)
	{
		Rect2 rect = Rect2(pos.x * mPixelSize, pos.y * mPixelSize, mPixelSize, mPixelSize);
		draw_rect(rect, *color);
	}

	void SceneGenerator::drawLine(Vector2 origin, Vector2 dest, Color* color, int flag /*= 0*/)
	{
		// flag == 0 : force pixel to be colored
		// flag == 1 : only change pixel if it was previously colored
		// flag == 2 : only change pixel if it was NOT previously colored

		Array linePoints = getLine(origin, dest);

		for (int i = 0; i < linePoints.size(); i++)
		{
			drawPixel(linePoints[i], color);//, flag);
		}
	}

	Array SceneGenerator::getLine(Vector2 origin, Vector2 dest)
	{
		Array pointsOnLine;

		float stepSize = 1.0f / mScreenSizePixel.x;

		Vector2 dir = dest - origin;

		// If a line is more vertical than horizontal, it should not have more than 1 pixel in same y coord. aka jaggies
		bool isVertical;
		if (abs(dir.y) >= abs(dir.x))
			isVertical = true;
		else
			isVertical = false;

		Vector2 temp = origin;

		// Holds prev XY coord, depending on if line is horizontal or vertical, which is then used to remove jaggies
		int lastXY = -1;
		int thisXY = -1;

		// Coef of line, 0 is first pixel, 1 is last pixel
		float coef = 0.0f;

		while (coef <= 1.0f)
		{
			temp = Vector2(
				round(origin.x * (1.0f - coef) + dest.x * coef),
				round(origin.y * (1.0f - coef) + dest.y * coef)
			);

			coef += stepSize;

			// Early out of we are outside the pixelArray
			if (temp.x < 0.0f || temp.x >= mScreenSizePixel.x || temp.y < 0.0f || temp.y >= mScreenSizePixel.y)
				continue;

			if (isVertical)
				thisXY = temp.y;
			else
				thisXY = temp.x;

			if (thisXY != lastXY)
			{
				pointsOnLine.append(temp);
				lastXY = thisXY;
			}
		}

		return pointsOnLine;
	}

	Color* SceneGenerator::getAvgColor(Color* color1, Color* color2, float ratio)
	{
		float subRatio = 1.0 - ratio;

		float avgR = (color1->r * ratio + color2->r * subRatio);
		float avgG = (color1->g * ratio + color2->g * subRatio);
		float avgB = (color1->b * ratio + color2->b * subRatio);
		float avgA = (color1->a * ratio + color2->a * subRatio);

		return new Color(avgR, avgG, avgB, avgA);
	}

	AssetGenerator* SceneGenerator::createAsset(Biome biomeType, bool forcePrimary /*= false*/)
	{
		AssetGenerator* assetGen;

		if (biomeType == BiomeCloud)
		{
			RockGenerator* rockGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			rockGen->setType(RockGenerator::RockTypeCloud);
			assetGen = rockGen;
		}
		else if (biomeType == BiomeMountain)
		{
			float ratio = 1 - std::min(mDistance, 1.0f);

			mCurrentMountainHeight = int(mBiomeMountainHeights[mInitBiome] * ratio + mBiomeMountainHeights[mDestBiome] * (1.0-ratio));

			Color* c0 = getAvgColor(mGroundColors[mInitBiome][0], mGroundColors[mDestBiome][0], ratio);
			Color* c1 = getAvgColor(mGroundColors[mInitBiome][mGroundColors[mInitBiome].size() - 1], mGroundColors[mDestBiome][mGroundColors[mDestBiome].size() - 1], ratio);

			RockGenerator* rockGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			rockGen->addMountain(mCurrentMountainHeight, (mCurrentMountainHeight >= 15 ? mGroundColors[BiomeMountain][0] : c0), (mCurrentMountainHeight >= 15 ? mGroundColors[BiomeMountain][1] : c1));
			assetGen = rockGen;
		}
		else if (biomeType == BiomeGrass)
		{
			if (!forcePrimary && rand() % 5 == 0)
			{
				RockGenerator* rockGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
				rockGen->setType(RockGenerator::RockTypeBush);
				assetGen = rockGen;
			}
			else
			{
				TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
				treeGen->setType(TreeGenerator::TreeTypeDeciduous);
				assetGen = treeGen;
			}
		}
		else if (biomeType == BiomeBoreal)
		{
			TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
			treeGen->setType(TreeGenerator::TreeTypeConiferous);
			assetGen = treeGen;
		}
		else if (biomeType == BiomeSnow)
		{
			TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
			treeGen->setType(TreeGenerator::TreeTypeSnowConiferous);
			assetGen = treeGen;
		}
		else if (biomeType == BiomeSand)
		{
			if (!forcePrimary && rand() % 5 == 0)
			{
				RockGenerator* rockGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
				rockGen->setType(RockGenerator::RockTypeGraphite);
				assetGen = rockGen;
			}
			else
			{
				TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(mTreeGenScene->instance());
				treeGen->setType(TreeGenerator::TreeTypeCactus);
				assetGen = treeGen;
			}
		}
		else if (biomeType == BiomeRock)
		{
			RockGenerator* rockGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			rockGen->setType(RockGenerator::RockTypeGraphite);
			assetGen = rockGen;
		}
		else
		{
			RockGenerator* rockGen = Object::cast_to<RockGenerator>(mRockGenScene->instance());
			rockGen->setType(RockGenerator::RockTypeGraphite);
			assetGen = rockGen;
		}

		return assetGen;
	}

}

