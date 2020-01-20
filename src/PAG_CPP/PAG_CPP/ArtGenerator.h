#pragma once

#include <core/Godot.hpp>
#include <Node2D.hpp>
#include <vector>
#include <set>



namespace godot
{

	class ArtGenerator : public Node2D
	{
		GODOT_CLASS(ArtGenerator, Node2D)

	public:
		ArtGenerator();
		~ArtGenerator();
		static void _register_methods();

		void _init();
		void ready();
		void draw();

	private:

		void drawPixel(Vector2 pos, Color* color);
		void drawPixelArray();

		void setPixel(Vector2 pos, Color* color);
		void addLine(Vector2 origin, Vector2 dest, Color* color);
		void addCircle(Vector2 origin, float radius, int samples, Color* color);
		void addShape(std::vector<Vector2> points, Color* lineColor, Color* fillColor = nullptr);
		void fillColor(Vector2 origin, Color* destColor, Color* origColor = nullptr);

		void resetPixelArray();
		void pivotPixelArray();

		void findLikeNeighbours(Vector2 origin, std::set<Vector2> &validNeighbours, std::set<Vector2> &toSearch, Color* origColor = nullptr);


		const Color* BLACK = new Color(0.0f, 0.0f, 0.0f, 1.0f);

		int mAssetSize;
		float mPixelSize;
		Vector2 mScreenSize;

		std::vector<std::vector<Color*>> mPixelArray;


	};




}