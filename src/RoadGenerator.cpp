#include "RoadGenerator.h"

using namespace godot;


void RoadGenerator::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_length"), &RoadGenerator::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "length"), &RoadGenerator::set_length);
	ClassDB::add_property("RoadGenerator", PropertyInfo(Variant::INT, "length"), "set_length", "get_length");
}

RoadGenerator::RoadGenerator()
{

	lsystem = nullptr;
	positions = TypedArray<Vector2i>();

	length = 8;


}

RoadGenerator::~RoadGenerator()
{
	//delete lsystem;
}

void RoadGenerator::_ready()
{
	LSystemGenerator* ls = get_node<LSystemGenerator>("LSystemGenerator");
	if (ls)
	{
		lsystem = ls;
	}
	else
	{
		UtilityFunctions::print("LSystemGenerator not found.");
		lsystem = nullptr;
	}
}

TypedArray<Vector2i> RoadGenerator::generateRoad(Vector2i startingPosition)
{
	if (!lsystem) 
		return TypedArray<Vector2i>();

	int curLength = get_length();
	positions.clear();

	String sequence = lsystem->generateSentence();

	std::stack<AgentParameters> savePoints; // сохраненные позиции 
	Vector2i currentPosition = startingPosition; // текущая позиция 
	Vector2i currentDirection = AgentParameters::directionRight; // текущее направление
	Vector2i tempPosition = Vector2i(0, 0); // используется при рисовании 
	bool currentDraw = true;
	positions.append(currentPosition);

	PackedStringArray letters = sequence.split();
	
	for (int i = 0; i < letters.size(); i++)
	{
		String letter = letters[i];
		// save
		if (letter == "[")
		{
			savePoints.push(AgentParameters(currentPosition, currentDirection, curLength, currentDraw));
		}
		// load
		else if (letter == "]")
		{
			if (savePoints.size() > 0)
			{
				AgentParameters agentParameter = savePoints.top();
				savePoints.pop();
				currentPosition = agentParameter.get_position();
				currentDirection = agentParameter.get_direction();
				curLength = agentParameter.get_lenght();
				currentDraw = agentParameter.get_draw();
			}
			else
			{
				UtilityFunctions::print("error with the stack");
			}
		}
		// go forward
		else if (letter == "F")
		{
			if (currentDraw && curLength > 0)
			{
				if (!drawLine(currentPosition, currentDirection, curLength))
				{
					currentDraw = false;
				}
			}
			
			currentPosition += currentDirection * curLength;

			RandomNumberGenerator random;
			random.randomize();
			curLength *=  1.5;
			if (curLength > 0)
				curLength = random.randi() % length + length / 10;

		}
		// turn right
		else if (letter == "+")
		{
			currentDirection = AgentParameters::turnRight(currentDirection);
		}
		// rurn left
		else if (letter == "-")
		{
			currentDirection = AgentParameters::turnLeft(currentDirection);
		}
		else
		{
			UtilityFunctions::print("wrong letter: ", letter);
			UtilityFunctions::print("The letter in the sequence is unknown.");
		}
	}
	return positions;
}

void RoadGenerator::set_length(const int l)
{
	if (l >= 0)
		length = l;
	else
		length = 0;

}

int RoadGenerator::get_length() const
{
	return length;
}

bool RoadGenerator::drawLine(Vector2i pos, Vector2i dir, int l)
{
	Vector2i newPos = pos;
	TypedArray<Vector2i> linePositions;
	bool b = true;
	for (int i = 0; i < l + 1; i++)
	{
		TypedArray<Vector2i> neihbors = GetSideNeighbors(newPos, dir);
		if (i > 0 && neihbors.size() != 2)
		{
			b = false;
			break;
		}
		newPos += dir;
		linePositions.append(Vector2i(round(newPos.x), round(newPos.y)));
		
	}
	if (b)
	{
		for (int i = 0; i < linePositions.size(); i++)
		{
			positions.append(linePositions[i]);
		}
	}
	return b;
}

TypedArray<Vector2i> RoadGenerator::GetSideNeighbors(Vector2i pos, Vector2i dir)
{
	TypedArray<Vector2i> neighbors;
	Vector2i neighbor = pos + AgentParameters::turnLeft(dir);

	if (positions.find(neighbor) == -1)
		neighbors.append(neighbor);

	neighbor = pos + AgentParameters::turnRight(dir);

	if (positions.find(neighbor) == -1)
		neighbors.append(neighbor);
	return neighbors;
}
