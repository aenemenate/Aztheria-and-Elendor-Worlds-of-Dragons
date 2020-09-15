#pragma once

#include <algorithm>
#include <functional>
#include <unordered_set>
#include <queue>
#include <array>
#include <vector>

#include "Node.h"

using namespace std;

/* Simple Grid class representing a Grid like 2D Map, In the future versions This class will be merged with Map class to which will offer multi dimensional support. 
* Constructor should initialize num of all the nodes in a grid, initalize directions, moevement type,  
*/
class Grid
{
public:

	//current movement type support, it will eventually support 
	//TODO: Rename it to Diagonal and straight so naming would be more consistent with multi dimensional maps

	enum movementType{ FOUR_DIRECTIONS, EIGHT_DIRECTIONS }; // add hexa

    Grid() {}
	Grid(int numOfColums, int numOfRows, movementType movType, vector<Node> obstacles);
	Grid(int numOfColums, int numOfRows, movementType movType, unordered_set<Node> obstacles);


	inline bool isWithinGrid(Node node) const; //check if node can be placed withing the grid
	bool isPassable(Node node) const; //check if the node is an obstacle or not.
	vector<Node> getNeighbors(Node node) const; //WARNIG MEM CREATED IN HEAP MAKE SURE YOU DELETE IT PROPERLY
	void generatePossibleDirections();

    int getWidth() { return m_numOfCols; }
	int getHeight() { return m_numOfRows; }

protected:
	/* Only to be called by the derived class. */
	Grid(int numOfColums, int numOfRows, movementType movType);

	/* TODO: Store Node pointers instead, and implement comparator functions to compare objects pointed by the pointers*/
	unordered_set<Node, std::hash<Node>> m_obstacles; // keeps track of all the obstacle nodes

	/*width and height of the grid change the name to rows and cols later for clearity */
	int m_numOfRows;
	int m_numOfCols;

	/* possible directions to visit, depends on dimension of the map and movement type.
	 * TODO: change this to accommodate multi dimensions
	 */
	vector<tuple<int,int>> m_directions; 
	movementType m_movement;

};

