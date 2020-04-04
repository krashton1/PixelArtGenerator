#include "Player.h"
#include "AssetGenerator.h"
#include "RockGenerator.h"
#include "TreeGenerator.h"

#include "SceneGenerator.h"

#include "Main.h"

using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options* o) {
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options* o) {
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void* handle) {
	Godot::nativescript_init(handle);
	register_class<AssetGenerator>();
	register_class<RockGenerator>();
	register_class<TreeGenerator>();
	
	register_class<SceneGenerator>();

	register_class<Player>();
	register_class<Main>();
}