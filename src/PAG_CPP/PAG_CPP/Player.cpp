#include "Player.h"

using namespace godot;

void godot::Player::_register_methods()
{
	register_method((char*)"_physics_process", &Player::PhysicsProcess);
	register_method((char*)"_ready", &Player::_ready);
}

void godot::Player::_init()
{
	//Dont need anything
}

void godot::Player::_ready()
{
	// Nothing
}

godot::Player::Player()
{
	// Nothing
}

godot::Player::~Player()
{
	//Dont need anything
}

void godot::Player::PhysicsProcess(float delta)
{
	UpdateMotionFromInput();

	m_Motion = move_and_slide(m_Motion, FLOOR);
}

void godot::Player::UpdateMotionFromInput()
{
	Input* input = Input::get_singleton();

	if (input->is_action_pressed("ui_right"))
	{
		m_Motion.x = MAX_SPEED;
	}
	if (input->is_action_pressed("ui_left"))
	{
		m_Motion.x = -MAX_SPEED;
	}
}
