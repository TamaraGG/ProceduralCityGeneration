#include "MyTileMap.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <vector>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/tile_map_pattern.hpp>
#include <godot_cpp/classes/tile_set.hpp>
#include "GodotAStar.h"


using namespace godot;

void MyTileMap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("generate_map"), &MyTileMap::generate_map);
	ClassDB::bind_method(D_METHOD("find_path", "dijkstra", "astar"), &MyTileMap::find_path);

	ClassDB::bind_method(D_METHOD("turn_on_input"), &MyTileMap::turn_on_input);
	ClassDB::bind_method(D_METHOD("turn_off_input"), &MyTileMap::turn_off_input);

	ADD_SIGNAL(MethodInfo("time_dijkstra_changed", PropertyInfo(Variant::STRING, "time")));
	ADD_SIGNAL(MethodInfo("time_a_star_changed", PropertyInfo(Variant::STRING, "time")));

}

MyTileMap::MyTileMap() 
{
	roadGenerator = nullptr;
}

void MyTileMap::_ready()
{
	Ref<TileSet> ts = get_tileset();
	
	if (!ts.is_null())
	{
		tileSet = ts;
	}
	else
	{
		UtilityFunctions::print("TileSet not found");
		return;
	}

	if (tileSet->get_custom_data_layers_count() == 0)
	{
		tileSet->add_custom_data_layer(0);
		tileSet->set_custom_data_layer_type(0, Variant::INT);
		tileSet->set_custom_data_layer_name(0, "road_sides");
	}

	set_position(Vector2(0, 0));	

	RoadGenerator* rg = get_node<RoadGenerator>("RoadGenerator");
	if (rg)
	{
		roadGenerator = rg;
	}
	else
	{
		UtilityFunctions::print("RoadGenerator not found.");
		roadGenerator = nullptr;
	}

	roadLayer = 0;
	treesLayer = 4;
	housesLayer = 3;
	pathLayer[0] = 1;
	pathLayer[1] = 2;
	pathLayer[2] = 3;

	roadTerrain = 0;
	roadTerrainSet = 1;
	groundTerrain = 0;
	groundTerrainSet = 0;
	pathTerrainSet = 1;
	pathTerrain[0] = 1;
	pathTerrain[1] = 2;

	TypedArray<Vector2i> array2;
	array2.append(Vector2i(0, 0));
	array2.append(Vector2i(0, 3));
	array2.append(Vector2i(0, 5));
	array2.append(Vector2i(0, 7));
	houseStartPoint[2] = array2;

	TypedArray<Vector2i> arr1;
	arr1.append(Vector2i(0, 0));
	arr1.append(Vector2i(2, 0));
	arr1.append(Vector2i(0, 2));
	arr1.append(Vector2i(4, 0));
	arr1.append(Vector2i(4, 1));
	arr1.append(Vector2i(5, 1));
	arr1.append(Vector2i(5, 0));
	arr1.append(Vector2i(6, 0));
	treeStartPoint[1] = arr1;

	if (tileSet->get_source_count() == 0)
	{
		UtilityFunctions::print("source not found in tileset.");
	}

	inputEnabled = true;
}

void MyTileMap::_input(const Ref<InputEvent>& event)
{
	if (!inputEnabled) return;
	Ref<InputEventMouseButton> key_event = event;
	if (key_event.is_valid() && key_event->is_pressed())
	{
		Vector2i mousePosition = local_to_map(get_global_mouse_position());
		
		TileData* data;
		switch (key_event->get_button_index())
		{
		case MOUSE_BUTTON_LEFT:
			data = get_cell_tile_data(roadLayer, mousePosition);
			
			if (data)
			{
				if (data->get_custom_data("road_sides"))
				{
					if (pathTiles.size() == 2 && (get_used_cells(pathLayer[0]).size() != 0 || get_used_cells(pathLayer[1]).size() != 0))
					{
						clear_layer(pathLayer[0]);
						clear_layer(pathLayer[1]);
						erase_cell(pathLayer[2], pathTiles[0]);
						erase_cell(pathLayer[2], pathTiles[1]);
						pathTiles.clear();
						emit_signal("time_dijkstra_changed", "");
						emit_signal("time_a_star_changed", "");
					}
					int x = pathTiles.find(mousePosition);
					if (x != -1)
					{
						pathTiles.remove_at(x);
						erase_cell(pathLayer[2], mousePosition);
					}
					else
					{
						if (pathTiles.size() == 2)
						{
							clear_layer(pathLayer[0]);
							clear_layer(pathLayer[1]);
							erase_cell(pathLayer[2], pathTiles[0]);
							erase_cell(pathLayer[2], pathTiles[1]);
							pathTiles.clear();
						}
						if (pathTiles.size() < 2)
						{
							
							set_cell(pathLayer[2], mousePosition, tileSetPathSourceID, pathPoint);
							pathTiles.append(mousePosition);
						}
					}
					
					
				}
			}
			break;

		case MOUSE_BUTTON_RIGHT:
			break;
		default:

			break;
		}
	}
}

void MyTileMap::generate_map()
{
	//clear();
	clear_layer(roadLayer);
	clear_layer(treesLayer);
	clear_layer(pathLayer[0]);
	clear_layer(pathLayer[1]);
	clear_layer(housesLayer);
	emit_signal("time_dijkstra_changed", "");
	emit_signal("time_a_star_changed", "");
	pathTiles.clear();

	
	if (tileSet.is_null())
	{
		UtilityFunctions::print("here");
		return;
	}

	road.clear();
	
	road = roadGenerator->generateRoad(Vector2(0,0));
	set_cells_terrain_connect(roadLayer, road, roadTerrainSet, roadTerrain);
	
	road = get_used_cells(roadLayer);
	
	placeHouses(road);
	roadGraph = arrayFromVectors(road, roadLayer);
}

void MyTileMap::placeHouses(TypedArray<Vector2i>& roadCells)
{
	Vector2i roadCell;
	RandomNumberGenerator random;

	int roadSize = roadCells.size();
	Vector2i curCell = roadCell;
	for (int i = 0; i < roadSize; i++)
	{
		TileData* data;
		TileData* houseData;
		
		roadCell = roadCells[i];
		data = get_cell_tile_data(roadLayer, roadCells[i]);
		
		if (data)
		{
			int roadSides = data->get_custom_data("road_sides");

			for (int j = 0; j < 4; j++)
			{
				TileSet::CellNeighbor dir; // направление дома (не двери)
					
				if (j == 3) dir = TileSet::CellNeighbor(0);
				else dir = TileSet::CellNeighbor(4 * (j + 1));
					
				int count = 0;
				Vector2i curRoadCell = roadCell;

				for (int k = 0; k < 3; k++)
				{
					houseData = get_cell_tile_data(housesLayer, get_neighbor_cell(curRoadCell,
								TileSet::CellNeighbor(4 * j)));
					data = get_cell_tile_data(roadLayer, curRoadCell);

					if (data)
					{
						int curRoadSides = data->get_custom_data("road_sides");
						if (!(curRoadSides & (1 << j)) && (!houseData || !houseData->get_custom_data("is_house")))
						{

							count++;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
					curRoadCell = get_neighbor_cell(curRoadCell, dir);
				}

				if (count >= 3 && random.randf() < 0.4)
				{
					placeOneHouse(roadCell, 3, TileSet::CellNeighbor(4 * j));
				}
				else if (count >= 2 && random.randf() < 0.4)
				{
					placeOneHouse(roadCell, 2, TileSet::CellNeighbor(4 * j));
				}
				else if (count >= 2 && random.randf() < 0.8)
				{
					placeOneTree(roadCell, 2, TileSet::CellNeighbor(4 * j));
				}
				else if (count >= 1 && random.randf() < 0.4)
				{
					placeOneHouse(roadCell, 1, TileSet::CellNeighbor(4 * j));
				}
				else if (count >= 1)
				{
					placeOneTree(roadCell, 1, TileSet::CellNeighbor(4 * j));
				}
					
			}

		}
		

	}
}

void MyTileMap::placeOneHouse(Vector2i position, int size, TileSet::CellNeighbor side)
{
	RandomNumberGenerator random;
	random.randomize();
	if (!houseStartPoint.has(size)) return;

	TypedArray<Vector2i> array = houseStartPoint[size];
	Vector2i houseStart = array[random.randi() % array.size()];

	Vector2i direction;
	switch (side)
	{
	case TileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
		direction = Vector2i(-1, 0);
		houseStart += Vector2i(size * 2 - 1, 0);
		break;
	case TileSet::CELL_NEIGHBOR_RIGHT_SIDE:
		direction = Vector2i(0, 1);
		houseStart += Vector2i(size * 2, 0);
		break;
	case TileSet::CELL_NEIGHBOR_TOP_SIDE:
		direction = Vector2i(1, 0);
		break;
	case TileSet::CELL_NEIGHBOR_LEFT_SIDE:
		direction = Vector2i(0, -1);
		houseStart += Vector2i(size * 3, 1);
		break;
	default:
		break;
	}

	position = get_neighbor_cell(position, side);
	for (int i = 0; i < size; i++)
	{
		set_cell(housesLayer, position + direction * i, tileSetHousesSourceID, houseStart + direction * i);
	}
}

void MyTileMap::placeOneTree(Vector2i position, int size, TileSet::CellNeighbor side)
{
	RandomNumberGenerator random;
	random.randomize();
	if (!treeStartPoint.has(size)) return;

	TypedArray<Vector2i> array = treeStartPoint[size];
	Vector2i treeStart = array[random.randi() % array.size()];

	Vector2i direction;
	switch (side)
	{
	case TileSet::CELL_NEIGHBOR_BOTTOM_SIDE:
		direction = Vector2i(-1, 0);
		break;
	case TileSet::CELL_NEIGHBOR_RIGHT_SIDE:
		direction = Vector2i(0, 1);
		treeStart += Vector2i((size - 1)* 2, 0);
		break;
	case TileSet::CELL_NEIGHBOR_TOP_SIDE:
		direction = Vector2i(1, 0);
		break;
	case TileSet::CELL_NEIGHBOR_LEFT_SIDE:
		direction = Vector2i(0, -1);
		treeStart += Vector2i((size - 1) * 2, 0);
		break;
	default:
		break;
	}

	position = get_neighbor_cell(position, side);
	for (int i = 0; i < size; i++)
	{
		set_cell(housesLayer, position + direction * i, tileSetTreesSourceID, treeStart + direction * i);
	}
}

void MyTileMap::turn_on_input()
{
	inputEnabled = true;
}

void MyTileMap::turn_off_input()
{
	inputEnabled = false;
}

std::vector<std::vector<int>> MyTileMap::arrayFromVectors(TypedArray<Vector2i>& vectors, int layer)
{
	int count = vectors.size();
	std::vector<std::vector<int>> graph(count);


	for (int i = 0; i < count; i++)
	{
		graph[i].assign(count, 0); 
		for (int j = 0; j < i; j++)
		{
			Vector2i v1 = vectors[j];
			Vector2i v2 = vectors[i];
			Vector2i v = v1 - v2;
			if (v.length() == 1)
			{
				graph[i][j] = 1;
				graph[j][i] = 1;

			}
		}
	}
	
	return graph;
}

TypedArray<Vector2i> MyTileMap::findPathDijkstra(Vector2i start, Vector2i end)
{
	int startInt = road.find(pathTiles[0]);
	int endInt = road.find(pathTiles[1]);

	auto beginTime = std::chrono::steady_clock::now();

	std::vector<int> pathArray = DijkstraAlgorithm::findPath(roadGraph, startInt, endInt);

	auto endTime = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime);
	long long time = elapsed_ms.count();
	
	emit_signal("time_dijkstra_changed", String::num_int64(time));

	TypedArray<Vector2i> pathVectors;
	for (int x : pathArray)
	{
		pathVectors.append(road[x]);
	}
	return pathVectors;
}

TypedArray<Vector2i>  MyTileMap::findPathAstar(Vector2i start, Vector2i end)
{
	TypedArray<Vector2i> pathVectors;
	GodotAStar* astar = get_node<GodotAStar>("../GodotAStar");
	if (astar)
	{
		auto beginTime = std::chrono::steady_clock::now();

		PackedVector2Array p = astar->findPath(map_to_local(pathTiles[0]), map_to_local(pathTiles[1]));

		auto endTime = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime);
		long long time = elapsed_ms.count();
		emit_signal("time_a_star_changed", String::num_int64(time));

		for (int i = 1; i < p.size()-1; i++)
		{
			pathVectors.append(local_to_map(p[i]));
		}
	}
	return pathVectors;
}

void MyTileMap::find_path(bool dijkstra, bool astar)
{
	if (pathTiles.size() == 2)
	{
		Vector2i start = pathTiles[0];
		Vector2i end = pathTiles[1];
		if (dijkstra)
			set_cells_terrain_connect(pathLayer[0], findPathDijkstra(start, end), pathTerrainSet, pathTerrain[0]);
		if (astar)
			set_cells_terrain_connect(pathLayer[1], findPathAstar(start, end), pathTerrainSet, pathTerrain[1]);
	}
}
