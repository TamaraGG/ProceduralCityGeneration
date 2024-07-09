#ifndef ROADGENERATOR_H
#define ROADGENERATOR_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/tile_map.hpp>
#include "godot_cpp/classes/label.hpp"
#include <godot_cpp/core/math.hpp>
#include <stack>
#include "LSystemGenerator.h"
#include "AgentParameters.h"
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include "DijkstraAlgorithm.h"


namespace godot
{
	class RoadGenerator : public Node2D {
		GDCLASS(RoadGenerator, Node2D)

	private:
		LSystemGenerator* lsystem;
		TypedArray<Vector2i> positions;

		int length;


	protected:
		static void _bind_methods();

	public:
		RoadGenerator();
		~RoadGenerator();

		void _ready();

		TypedArray<Vector2i> generateRoad(Vector2i startingPosition);

		void set_length(const int l);
		int get_length() const;

	private:
		bool drawLine(Vector2i pos, Vector2i dir, int l);
		TypedArray<Vector2i> GetSideNeighbors(Vector2i pos, Vector2i dir);
	};

}


#endif