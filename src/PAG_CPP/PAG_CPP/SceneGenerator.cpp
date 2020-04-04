#include "SceneGenerator.h"
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include "TreeGenerator.h"

namespace godot
{

	SceneGenerator::SceneGenerator(Vector2 screenSize /*= Vector2(1024, 1024)*/, int pixelSize /*= 16*/)
		: mScreenSize(screenSize)
		, mPixelSize(pixelSize)
		, mDistance(0.0f)
	{
		mScreenSizePixel = Vector2(mScreenSize.x / mPixelSize, mScreenSize.y / mPixelSize);
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
		// Do nothing
	}

	void SceneGenerator::_ready()
	{
		// Do nothing
	}

	void SceneGenerator::_draw()
	{
		//drawPixel(Vector2(30, 30), new Color(1, 0, 0));
		Ref<PackedScene> treeGenScene = ResourceLoader::get_singleton()->load("res://TreeGenerator.tscn");



		float bandPos[11] = {
			0,
			mScreenSizePixel.y / 10 * 2.5,
			mScreenSizePixel.y / 10 * 2.75,
			mScreenSizePixel.y / 10 * 3,
			mScreenSizePixel.y / 10 * 3.25,
			mScreenSizePixel.y / 10 * 3.6,
			mScreenSizePixel.y / 10 * 4.1,
			mScreenSizePixel.y / 10 * 4.8,
			mScreenSizePixel.y / 10 * 5.7,
			mScreenSizePixel.y / 10 * 6.8,
			mScreenSizePixel.y / 10 * 8.1,
		};

		// Draw bands from each layer (so objects from each layer overlap)

		int i = 0;
		for (std::vector<std::vector<Color*>> band : mLayerGrounds)
		{
			for (int x = 0; x < band.size(); x++)
			{
				for (int y = 0; y < band[x].size(); y++)
				{
					drawPixel(Vector2(x, y + bandPos[i]), band[x][y]);
				}
			}
			i++;
		}


		//// Draw Layer 10 (Sky)

		//int skyBound = mScreenSizePixel.y / 4;

		//for (int y = 0; y < skyBound; y++)
		//{
		//	for (int x = 0; x < mScreenSizePixel.x; x++)
		//	{
		//		//drawPixel(Vector2(x, y), new Color(0, 0.5, 1.0));
		//	}
		//}

		//// Add Sun
		//// Add Clouds

		//// Draw Layer 9 (Background)

		//for (int i = 0; i < 10; i++)
		//{
		//	drawLine(Vector2(mScreenSizePixel.x / 10 * (i), skyBound), Vector2(mScreenSizePixel.x / 10 * (i + 0.5), skyBound / 2), new Color(1.0, 0.3, 0.3), 0);
		//	drawLine(Vector2(mScreenSizePixel.x / 10 * (i + 0.5), skyBound / 2), Vector2(mScreenSizePixel.x / 10 * (i + 1), skyBound), new Color(1.0, 0.3, 0.3), 0);

		//}



		//

		//// Draw Layer 8 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.03, 0.03));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 0 * mPixelSize, mScreenSizePixel.y / 10 * 2.75 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 7 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.05, 0.05));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 0.5 * mPixelSize, mScreenSizePixel.y / 10 * 3 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 6 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.08, 0.08));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 1 * mPixelSize, mScreenSizePixel.y / 10 * 3.25 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 5 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.12, 0.12));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 1.5 * mPixelSize, mScreenSizePixel.y / 10 * 3.6 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 4 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.17, 0.17));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 2 * mPixelSize, mScreenSizePixel.y / 10 * 4.1 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 3 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.23, 0.23));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 2.8 * mPixelSize, mScreenSizePixel.y / 10 * 4.8 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 2 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.30, 0.30));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 4 * mPixelSize, mScreenSizePixel.y / 10 * 5.7 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 1 (Object Layer)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.38, 0.38));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 5 * mPixelSize, mScreenSizePixel.y / 10 * 6.8 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}

		//// Draw Layer 0 (Foreground)

		//for (int i = 0; i < 1; i++)
		//{
		//	TreeGenerator* treeGen = Object::cast_to<TreeGenerator>(treeGenScene->instance());
		//	treeGen->apply_scale(Vector2(0.45, 0.45));
		//	treeGen->set_position(Vector2(mScreenSizePixel.x / 10 * 6.5 * mPixelSize, mScreenSizePixel.y / 10 * 8.1 * mPixelSize - treeGen->get_scale().y * 1024));
		//	add_child(treeGen);
		//}
	}

	void SceneGenerator::updateDistance()
	{



		float bandPos[11] = {
			mScreenSizePixel.y / 10 * 2.5 * mPixelSize,
			mScreenSizePixel.y / 10 * 2.75 * mPixelSize,
			mScreenSizePixel.y / 10 * 3 * mPixelSize,
			mScreenSizePixel.y / 10 * 3.25 * mPixelSize,
			mScreenSizePixel.y / 10 * 3.6 * mPixelSize,
			mScreenSizePixel.y / 10 * 4.1 * mPixelSize,
			mScreenSizePixel.y / 10 * 4.8 * mPixelSize,
			mScreenSizePixel.y / 10 * 5.7 * mPixelSize,
			mScreenSizePixel.y / 10 * 6.8 * mPixelSize,
			mScreenSizePixel.y / 10 * 8.1 * mPixelSize,
			mScreenSizePixel.y / 10 * 10 * mPixelSize
		};




		if (mDistance != 0.0)
		{
			for (int x = 1; x < mLayerGrounds[0].size(); x++)
			{
				mLayerGrounds[0][x - 1] = mLayerGrounds[0][x];
			}

			std::vector<Color*> tempCol;
			for (int y = 0; y < bandPos[0]; y++)
			{
				int r = rand() % 2;

				if (r == 0)
					tempCol.push_back(new Color(0, 0.5, 1));
				else
					tempCol.push_back(new Color(0, 0.3, 0.9));
			}

			mLayerGrounds[0][mLayerGrounds[0].size()-1] = tempCol;
			




			for (int i = 1; i < mLayerGrounds.size(); i++)
			{
				for (int x = 1; x < mLayerGrounds[i].size(); x++)
				{
					mLayerGrounds[i][x - 1] = mLayerGrounds[i][x];
				}
				

				std::vector<Color*> tempCol;
				for (int y = bandPos[i]; y < bandPos[i + 1]; y++)
				{
					int r = rand() % 2;

					if (r == 0)
						tempCol.push_back(new Color(0, 0.5, 0));
					else
						tempCol.push_back(new Color(0, 0.3, 0));
				}

				mLayerGrounds[i][mLayerGrounds[i].size()-1] = tempCol;
			}
		}

		mDistance = 0.0f;
	}

	void SceneGenerator::setup()
	{
		// Create sky tex


		float bandPos[11] = {
			mScreenSizePixel.y / 10 * 2.5 * mPixelSize,
			mScreenSizePixel.y / 10 * 2.75 * mPixelSize,
			mScreenSizePixel.y / 10 * 3 * mPixelSize,
			mScreenSizePixel.y / 10 * 3.25 * mPixelSize,
			mScreenSizePixel.y / 10 * 3.6 * mPixelSize,
			mScreenSizePixel.y / 10 * 4.1 * mPixelSize,
			mScreenSizePixel.y / 10 * 4.8 * mPixelSize,
			mScreenSizePixel.y / 10 * 5.7 * mPixelSize,
			mScreenSizePixel.y / 10 * 6.8 * mPixelSize,
			mScreenSizePixel.y / 10 * 8.1 * mPixelSize,
			mScreenSizePixel.y / 10 * 10 * mPixelSize
		};

		std::vector<std::vector<Color*>> skyTex;

		for (int x = 0; x < mScreenSizePixel.x; x++)
		{
			std::vector<Color*> tempCol;
			for (int y = 0; y < bandPos[0] / 4; y++)
			{
				int r = rand() % 2;

				if (r == 0)
					tempCol.push_back(new Color(0, 0.5, 1));
				else
					tempCol.push_back(new Color(0, 0.3, 0.9));
			}

			skyTex.push_back(tempCol);
		}

		mLayerGrounds.push_back(skyTex);



		// Create ground textures


		for (int i = 1; i < 10; i++)
		{


			std::vector<std::vector<Color*>> bandTex;

			for (int x = 0; x < mScreenSizePixel.x; x++)
			{
				std::vector<Color*> tempCol;
				for (int y = bandPos[i-1]; y < bandPos[i]; y++)
				{
					int r = rand() % 2;

					if (r == 0)
						tempCol.push_back(new Color(0, 0.5, 0));
					else
						tempCol.push_back(new Color(0, 0.3, 0));
				}

				bandTex.push_back(tempCol);
			}

			mLayerGrounds.push_back(bandTex);
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

}

