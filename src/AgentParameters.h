#pragma once

#ifndef AGENTPARAMETERS_H
#define AGENTPARAMETERS_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/node2d.hpp>



namespace godot
{
	class AgentParameters {

	private:
		Vector2i position;
		Vector2i direction;
		int length;
		bool draw;

	public:

		inline static const Vector2i directionBackward = Vector2i(0, -1);
		inline static const Vector2i directionRight = Vector2i(1, 0);
		inline static const Vector2i directionForward = Vector2i(0, 1);
		inline static const Vector2i directionLeft = Vector2i(-1, 0);


		AgentParameters();
		AgentParameters(Vector2i position, Vector2i direction, int lenght, bool draw);

		Vector2i get_position();
		void set_position(Vector2i p);
		Vector2i get_direction();
		void set_direction(Vector2i d);
		int get_lenght();
		void set_lenght(int l);
		bool get_draw();
		void set_draw(bool l);

		static Vector2i turnRight(Vector2i curDir);
		static Vector2i turnLeft(Vector2i curDir);
	};

}

#endif