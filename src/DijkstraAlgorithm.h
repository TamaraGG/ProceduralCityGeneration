#ifndef DIJKSTRAALGORITHM_H
#define DIJKSTRAALGORITHM_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>

namespace godot
{
	class DijkstraAlgorithm : public Object {
		GDCLASS(DijkstraAlgorithm, Object)
	private:
	protected:
		static void _bind_methods();
	public:
		static std::vector<int> findPath(std::vector<std::vector<int>> array, int start, int end);
	private:
		static int minDist(std::vector<int> distance, std::vector<bool> Tset);
	};
}
#endif