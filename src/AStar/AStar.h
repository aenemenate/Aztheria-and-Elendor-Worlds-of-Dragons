#pragma once

#include "Grid.h"
#include <unordered_map>
#include <unordered_set>

class AStar
{
public:
	AStar(Grid *grid);

	/* Implemented Heuristics */
	inline int ManhattanHeuristic(Node start, Node goal) const;
	inline double EuclideanHeuristic(Node start, Node goal) const;

	/* Performs Astar search to find the shortest between start and goal nodes.  */
	vector<Node> search(Node start, Node goal);

private:
	Grid *m_grid; //ptr to grid object

	/* Not happy with this OOD, it might be best to get rid of m_path and return a vector unique_ptr to Node object instead
	* Deep copying the path is detrimental to scalability! + I wanna make search funct const-correct.
	*/
	vector<Node> m_path;  //contains the shortest path from start to end node, make it so that it stores ptr to nodes
	void reconstructPath(unordered_map<Node, Node> closed, Node start, Node goal);
};

