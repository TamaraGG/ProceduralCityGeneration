#ifndef GODOTASTAR_H
#define GODOTASTAR_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/navigation_agent2d.hpp>
#include <godot_cpp/classes/a_star_grid2d.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

namespace godot
{

	class GodotAStar : public Node2D {
		GDCLASS(GodotAStar, Node2D)

	private:
		NavigationAgent2D* navAgent;
		Node2D* startNode;
		Node2D* targetNode;

	protected:
		static void _bind_methods();
		
	public:
		void _ready();
		PackedVector2Array findPath(Vector2 start, Vector2 finish);
	};

}

#endif