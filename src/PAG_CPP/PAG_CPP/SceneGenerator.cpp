#include "SceneGenerator.h"
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include "TreeGenerator.h"

#include <cmath>

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
		mBandPos[0]  = (mScreenSizePixel.y / 10.0f * 2.50f * mPixelSize) - mPixelSize;
		mBandPos[1]  = (mScreenSizePixel.y / 10.0f * 2.50f * mPixelSize) - mPixelSize;
		mBandPos[2]  = (mScreenSizePixel.y / 10.0f * 2.75f * mPixelSize) - mPixelSize;
		mBandPos[3]  = (mScreenSizePixel.y / 10.0f * 3.00f * mPixelSize) - mPixelSize;
		mBandPos[4]  = (mScreenSizePixel.y / 10.0f * 3.25f * mPixelSize) - mPixelSize;
		mBandPos[5]  = (mScreenSizePixel.y / 10.0f * 3.60f * mPixelSize) - mPixelSize;
		mBandPos[6]  = (mScreenSizePixel.y / 10.0f * 4.10f * mPixelSize) - mPixelSize;
		mBandPos[7]  = (mScreenSizePixel.y / 10.0f * 4.80f * mPixelSize) - mPixelSize;
		mBandPos[8]  = (mScreenSizePixel.y / 10.0f * 5.70f * mPixelSize) - mPixelSize;
		mBandPos[9]  = (mScreenSizePixel.y / 10.0f * 6.80f * mPixelSize) - mPixelSize;
		mBandPos[10]  = (mScreenSizePixel.y / 10.0f * 8.10f * mPixelSize) - mPixelSize;
		mBandPos[11] = (mScreenSizePixel.y / 10.0f * 10.0f * mPixelSize) - mPixelSize;

		// Make boundary between bands not interupt pixels
		for (int i = 0; i < 12; i++)
		{
			mBandPos[i] = mBandPos[i] - (fmod(mBandPos[i], mPixelSize));
		}



		// Scroll speed of each band in order to create parralax effect
		mBandScrollSpeed[0] = 0.00f; 
		mBandScrollSpeed[1] = 1.0f;
		mBandScrollSpeed[2] = 1.5f;
		mBandScrollSpeed[3] = 1.65f;
		mBandScrollSpeed[4] = 1.8f;
		mBandScrollSpeed[5] = 2.0f;
		mBandScrollSpeed[6] = 2.3f; 
		mBandScrollSpeed[7] = 2.8f; 
		mBandScrollSpeed[8] = 3.4f;
		mBandScrollSpeed[9] = 4.2f; 
		mBandScrollSpeed[10] = 5.0f; 
		mBandScrollSpeed[11] = 6.4f; 

		// Current position of each band, since we recycle the bands
		for (int i = 0; i < 12; i++)
		{
			mBandCurPos[i] = 0.0f;
		}
		

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
		Ref<PackedScene> treeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");








		TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.03, 0.03));
		add_child(treeGen);

		Asset newAsset;
		newAsset.band = 2;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.05, 0.05));
		add_child(treeGen);

		newAsset;
		newAsset.band = 3;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.05, 0.05));
		add_child(treeGen);

		newAsset;
		newAsset.band = 4;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.08, 0.08));
		add_child(treeGen);

		newAsset;
		newAsset.band = 5;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.12, 0.12));
		add_child(treeGen);

		newAsset;
		newAsset.band = 6;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.17, 0.17));
		add_child(treeGen);

		newAsset;
		newAsset.band = 7;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.23, 0.23));
		add_child(treeGen);

		newAsset;
		newAsset.band = 8;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.30, 0.30));
		add_child(treeGen);

		newAsset;
		newAsset.band = 9;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);


		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.38, 0.38));
		add_child(treeGen);

		newAsset;
		newAsset.band = 10;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);





		treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		treeGen->apply_scale(Vector2(0.45, 0.45));
		add_child(treeGen);

		newAsset.band = 11;
		newAsset.bandPos = 160;
		newAsset.asset = treeGen;

		mAssets.push_back(newAsset);



		
	}

	void SceneGenerator::_ready()
	{
		// Do nothing
	}

	void SceneGenerator::_draw()
	{
		//drawPixel(Vector2(30, 30), new Color(1, 0, 0));
		

		// Draw bands from each layer (so objects from each layer overlap)

		for (int i = 0; i < mBands.size(); i++)
		{
			for (int x = 0; x < mBands[i].size(); x++)
			{
				//int shiftX = (x + int(mBandCurPos[i])) % mBands[i].size();
				int newX = (x - int(mBandCurPos[i]));
				while (newX < 0)
					newX += mBands[i].size();
				newX = newX % mBands[i].size();

				for (int y = 0; y < mBands[i][x].size(); y++)
				{
					drawPixel(Vector2(newX, y + (i == 0 ? 0 : mBandPos[i-1]) / mPixelSize), mBands[i][x][y]);
				}
			}
		}

		
		for (Asset curAsset : mAssets)
		{

			//int x = int(((curAsset.bandPos - int(mBandCurPos[curAsset.band]))) * mPixelSize);
			int x = (curAsset.bandPos - int(mBandCurPos[curAsset.band])) * mPixelSize - (curAsset.asset->get_scale().x * 1024 / 2.0);
			int y = (mBandPos[curAsset.band] + mBandPos[curAsset.band - 1]) / 2 - (curAsset.asset->get_scale().y * 1024);

			curAsset.asset->set_position(Vector2(
				x - (x % mPixelSize),
				y - (y % mPixelSize)
			));
		}



	}

	void SceneGenerator::updateDistance()
	{

		if (mDistance != 0.0)
		{
			for (int i = 0; i < 12; i++)
			{
				mBandCurPos[i] += 1 * mBandScrollSpeed[i];
			}
		}

		mDistance = 0.0f;
	}

	void SceneGenerator::setup()
	{

		Ref<PackedScene> treeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");


		// Create sky tex

		std::vector<std::vector<Color*>> skyTex;

		for (int x = 0; x < mScreenSizePixel.x; x++)
		{
			std::vector<Color*> tempCol;
			for (int y = 0; y < mBandPos[0] / 4; y++)
			{
				int r = rand() % 2;

				if (r == 0)
					tempCol.push_back(new Color(.075, 0.705, 0.940));
				else
					tempCol.push_back(new Color(.075, 0.685, 0.910));
			}

			skyTex.push_back(tempCol);
		}

		mBands.push_back(skyTex);


		// Create background tex (this will be empty, bc sky tex meets ground tex. But background objects will be held in this band instead of sky band, since they scroll at a diff speed

		std::vector<std::vector<Color*>> backgroundTex;
		mBands.push_back(backgroundTex);





		// Create ground textures

		for (int i = 2; i < 12; i++)
		{
			std::vector<std::vector<Color*>> bandTex;

			for (int x = 0; x < mScreenSizePixel.x; x++)
			{
				std::vector<Color*> tempCol;
				for (int y = mBandPos[i-1]; y < mBandPos[i]; y++)
				{
					int r = rand() % 2;

					if (r == 0)
						tempCol.push_back(new Color(0.2, 0.6, 0.2));
					else
						tempCol.push_back(new Color(0.2, 0.5, 0.2));
				}

				bandTex.push_back(tempCol);
			}

			mBands.push_back(bandTex);
		}



		// Create Objects

		// Draw Layer 10 (Sky)

		// Add Sun
		// Add Clouds

		// Draw Layer 9 (Background)

		//for (int i = 0; i < 10; i++)
		//{
		//	drawLine(Vector2(mScreenSizePixel.x / 10 * (i), skyBound), Vector2(mScreenSizePixel.x / 10 * (i + 0.5), skyBound / 2), new Color(1.0, 0.3, 0.3), 0);
		//	drawLine(Vector2(mScreenSizePixel.x / 10 * (i + 0.5), skyBound / 2), Vector2(mScreenSizePixel.x / 10 * (i + 1), skyBound), new Color(1.0, 0.3, 0.3), 0);
		//}

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

}

