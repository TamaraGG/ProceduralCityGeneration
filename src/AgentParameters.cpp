#pragma once
#include "AgentParameters.h"

using namespace godot;

AgentParameters::AgentParameters()
{
	position = Vector2i(0,0);
	direction = directionForward;
	length = 1;
}

AgentParameters::AgentParameters(Vector2i p, Vector2i d, int l, bool dr)
{
	position = p;
	set_direction(d);
	set_lenght(l);
	draw = dr;
}

Vector2i AgentParameters::get_position()
{
	return position;
}

void AgentParameters::set_position(Vector2i p)
{
	position = p;
}

Vector2i AgentParameters::get_direction()
{
	return direction;
}

void AgentParameters::set_direction(Vector2i d)
{
	if (d.x * d.x + d.y * d.y == 1)
		direction = d;
	else direction = directionForward;
}

int AgentParameters::get_lenght()
{
	return length;
}

void AgentParameters::set_lenght(int l)
{
	if (l >= 0)
		length = l;
	else
		length = 0;
}

bool AgentParameters::get_draw()
{
	return draw;
}

void AgentParameters::set_draw(bool l)
{
	draw = l;
}

Vector2i AgentParameters::turnRight(Vector2i curDir)
{
	if (curDir == directionBackward)
		return directionRight;

	if (curDir == directionForward)
		return directionLeft;

	if (curDir == directionRight)
		return directionForward;

	if (curDir == directionLeft)
		return directionBackward;

	return curDir;
}

Vector2i AgentParameters::turnLeft(Vector2i curDir)
{
	if (curDir == directionBackward)
		return directionLeft;

	if (curDir == directionForward)
		return directionRight;

	if (curDir == directionRight)
		return directionBackward;

	if (curDir == directionLeft)
		return directionForward;

	return curDir;
}
