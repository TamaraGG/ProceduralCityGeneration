#ifndef MYTILEMAP_H
#define MYTILEMAP_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "RoadGenerator.h"
#include <godot_cpp/classes/tile_data.hpp>
#include <queue>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <chrono>

namespace godot {

class MyTileMap : public TileMap {
	GDCLASS(MyTileMap, TileMap)

private:
	RoadGenerator* roadGenerator;
	Ref<TileSet> tileSet;
	Label* timeDijkstraLabel;
	Label* timeAStarLabel;

	int roadLayer;
	int groundLayer;
	int treesLayer;
	int housesLayer;
	int pathLayer[3];

	TypedArray<Vector2i> pathTiles;
	TypedArray<Vector2i> road;

	int roadTerrainSet;
	int roadTerrain;
	int groundTerrainSet;
	int groundTerrain;
	int pathTerrainSet;
	int pathTerrain[2];
	std::vector<std::vector<int>> roadGraph;

	int tileSetRoadSourceID = 0;
	int tileSetGrasSourceID = 1;
	int tileSetPathSourceID = 2;
	int tileSetHousesSourceID = 3;
	int tileSetTreesSourceID = 4;

	Vector2i pathPoint = Vector2i(1,0);


	Dictionary houseStartPoint; 
	Dictionary treeStartPoint;

	bool inputEnabled;


protected:
	static void _bind_methods();

public:
	MyTileMap();

	void _ready();
	void _input(const Ref<InputEvent>& event);
	
	void generate_map();
	void find_path(bool dijkstra, bool astar);
	TypedArray<Vector2i> findPathDijkstra(Vector2i start, Vector2i end);
	TypedArray<Vector2i> findPathAstar(Vector2i start, Vector2i ends);

private:
	void placeHouses(TypedArray<Vector2i>& roadCells);
	void placeOneHouse(Vector2i position, int size, TileSet::CellNeighbor side);
	void placeOneTree(Vector2i position, int size, TileSet::CellNeighbor side);
	void turn_on_input();
	void turn_off_input();

	std::vector<std::vector<int>>  arrayFromVectors(TypedArray<Vector2i>& vectors, int layer);
};

}

#endif