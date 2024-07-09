#include "register_types.h"

#include "gdexample.h"
#include "MyTileMap.h"
#include "Rule.h"
#include "LSystemGenerator.h"
#include "RoadGenerator.h"
#include "DijkstraAlgorithm.h"
#include "MyCamera.h"
#include "GodotAStar.h"


#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<GDExample>();
	ClassDB::register_class<MyTileMap>();
	ClassDB::register_class<Rule>();
	ClassDB::register_class<LSystemGenerator>();
	ClassDB::register_class<RoadGenerator>();
	ClassDB::register_class<DijkstraAlgorithm>();
	ClassDB::register_class<MyCamera>();
	ClassDB::register_class<GodotAStar>();
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}