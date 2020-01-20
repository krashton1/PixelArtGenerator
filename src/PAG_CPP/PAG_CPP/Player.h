#pragma once

#include <core/Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Input.hpp>

namespace godot
{
	class Player : public KinematicBody2D
	{
		GODOT_CLASS(Player, KinematicBody2D)
	public:
		const int MAX_SPEED = 150;
		const int GRAVITY = 14;
		const int JUMP_FORCE = GRAVITY * -27;
		const Vector2 FLOOR = Vector2(0, -1);

		static void _register_methods();
		void _init();
		void _ready();

		Player();
		~Player();

		void PhysicsProcess(float delta);

		void UpdateMotionFromInput();

	private:
		Vector2 m_Motion;
	};

}