#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include <vector>
#include <list>
#include <set>



namespace godot
{

	class ArtGenerator : public Node2D
	{
		GODOT_CLASS(ArtGenerator, Node2D)

	public:
		ArtGenerator(Vector2 size = Vector2(1024,1024), int numPixels = 64);
		~ArtGenerator();

		virtual void setup(Vector2 pos = Vector2(0, 0), Vector2 size = Vector2(1024, 1024), int numPixels = 64);

		// Godot Functions
		static void _register_methods();
		virtual void _init();
		void _ready();
		void _draw();

		// Pixel Array Functions
		void setPixel(Vector2 pos, Color* color, int flag = 0);
		void addLine(Vector2 origin, Vector2 dest, Color* color);
		void addCircle(Vector2 origin, float radius, int samples, Color* color);
		void addShape(std::vector<Vector2> points, Color* lineColor, Color* fillColor = nullptr);
		void fillColor(Vector2 origin, Color* destColor, Color* origColor = nullptr);
		void sprayPixel(Vector2 origin, float size, float intensity, Color* color = nullptr, bool paintOver = false);

		// Pixel Array (whole) Functions
		void resetPixelArray();
		void rotatePixelArray();

		// Helper Functions
		static Array getLine(Vector2 origin, Vector2 dest);
		static bool compareColor(Color* color1, Color* color2);

		// Debug Pixel Array Functions
		void addSmile();

		// Accessors / Processors
		inline void setColorRamp(std::vector<Color*> newRamp) { mColorRamp = newRamp; }
		inline std::vector<Color*> getColorRamp() { return mColorRamp; }

	protected:

		int mAssetSize;
		float mPixelSize;
		Vector2 mScreenSize;

		Vector2 mPosition;
		Vector2 mSize;

		Color* mPixelArray[64][64];
		Color* mBlack;
		std::vector<Color*> mColorRamp;

	private:

		void drawPixel(Vector2 pos, Color* color);
		void drawPixelArray();

		void findLikeNeighbours(Vector2 origin, std::set<Vector2> &validNeighbours, std::set<Vector2> &toSearch, Color* origColor = nullptr);
	};




}