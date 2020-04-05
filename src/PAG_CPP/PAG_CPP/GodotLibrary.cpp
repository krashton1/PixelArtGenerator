#include "Player.h"
#include "AssetGenerator.h"
#include "RockGenerator.h"
#include "TreeGenerator.h"

#include "SceneGenerator.h"

#include "Main.h"

//using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options* o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options* o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
	godot::Godot::nativescript_init(handle);
	godot::register_class<godot::AssetGenerator>();
	godot::register_class<godot::RockGenerator>();
	godot::register_class<godot::TreeGenerator>();
	
	godot::register_class<godot::SceneGenerator>();

	godot::register_class<godot::Player>();
	godot::register_class<godot::Main>();
}