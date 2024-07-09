#include "DijkstraAlgorithm.h"

using namespace godot;

void DijkstraAlgorithm::_bind_methods()
{
	
}


std::vector<int> DijkstraAlgorithm::findPath(std::vector<std::vector<int>> array, int start, int finish)
{

    int count = array.size();

    std::vector<int> distance; // минимальное расстояние до каждой вершины
    distance.assign(count, INT_MAX);
    std::vector<bool> Tset; // true, если до вершины уже найден минимальный путь, false - не найден
    Tset.assign(count, false);
    std::vector<int> previous; // предшествующая вершина на минимальном пути
    previous.assign(count, -1);
    std::vector<int> res;
    int m = -1;

    distance[start] = 0;               

    for (int k = 0; k < count; k++)
    {
        m = minDist(distance, Tset);
        Tset[m] = true;
        if (m == finish)
            break;
        for (int k = 0; k < count; k++)
        {
            if (!Tset[k] && array[m][k] && distance[m] != INT_MAX && distance[m] + array[m][k] < distance[k])
            {
                distance[k] = distance[m] + array[m][k];
                previous[k] = m;
            }
        }
    }
    if (m == finish)
    {
        int curIdx = finish;
        while (curIdx != start)
        {
            res.push_back(curIdx);
            curIdx = previous[curIdx];
        } 
        res.push_back(start);
        
    }

    return res;
   
}

int DijkstraAlgorithm::minDist(std::vector<int> distance, std::vector<bool> Tset)
{
    int minimum = INT_MAX, ind;

    for (int k = 0; k < distance.size(); k++)
    {
        if (Tset[k] == false && distance[k] <= minimum)
        {
            minimum = distance[k];
            ind = k;
        }
    }
    return ind;
}

