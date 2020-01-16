#include "ArtGenerator.h"

namespace godot
{

ArtGenerator::ArtGenerator()
	: mPixelSize(16)
	, mAssetSize(64)
	, mScreenSize(Vector2(1280, 720)) 
{
	resetPixelArray();

	Color* black = new Color(BLACK.r, BLACK.g, BLACK.b);

	setPixel(Vector2(1, 1), black);
	setPixel(Vector2(1, 3), black);
	setPixel(Vector2(3, 0), black);
	setPixel(Vector2(3, 4), black);
	setPixel(Vector2(4, 1), black);
	setPixel(Vector2(4, 2), black);
	setPixel(Vector2(4, 3), black);
}

ArtGenerator::~ArtGenerator()
{

}

void ArtGenerator::_register_methods()
{
	register_method((char*)"_ready", &ready);
	register_method((char*)"_draw", &draw);
}

void ArtGenerator::_init()
{
	//if (0 == 1)
	//	return;
}

void ArtGenerator::ready()
{

}

void ArtGenerator::draw()
{
	drawPixelArray();
}

void ArtGenerator::drawPixel(Vector2 pos, Color* color)
{
	Rect2 rect = Rect2(pos.x, pos.y, mPixelSize, mPixelSize);
	draw_rect(rect, *color);
}

void ArtGenerator::drawPixelArray()
{
	for (int i = 0; i < mPixelArray.size(); i++) 
	{
		for (int j = 0; j < mPixelArray[i].size(); j++)
		{
			if (mPixelArray[i][j] != nullptr)
			{
				drawPixel(Vector2(i*mPixelSize, j*mPixelSize), mPixelArray[i][j]);
			}
		}
	}
}

void ArtGenerator::setPixel(Vector2 pos, Color* color)
{
	mPixelArray[pos.x][pos.y] = color;
}

void ArtGenerator::resetPixelArray()
{
	mPixelArray.resize(mAssetSize);
	for (int i = 0; i < mPixelArray.size(); i++)
	{
		mPixelArray[i].resize(mAssetSize);
		for (int j = 0; j < mPixelArray[i].size(); j++)
		{
			mPixelArray[i][j] = nullptr;
		}
	}
}

}

