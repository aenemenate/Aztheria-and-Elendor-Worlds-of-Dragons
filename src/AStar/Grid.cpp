#include "Grid.h"


Grid::Grid(int numOfColums, int numOfRows, movementType movType, vector<Node> obstacles)
{
	m_numOfCols = numOfColums;
	m_numOfRows = numOfRows;
	m_movement = movType;
	generatePossibleDirections();

	/* Copy from vector to unorederd_set */
	m_obstacles.insert(obstacles.begin(), obstacles.end());


}
/* Grid Constructor */
Grid::Grid(int numOfColums, int numOfRows, movementType movType, unordered_set<Node> obstacles)
{
	m_numOfCols = numOfColums;
	m_numOfRows = numOfRows;
	m_movement = movType;
	generatePossibleDirections();
	m_obstacles = obstacles;
}

/* This is a protected constructor meant to be called by the derived class NOT INCLUDED WITH THIS BUILD!. 
m_obstacles will be initialized in derived class therefore it will most likely cause a crash if it's called elsewhere.
*/
Grid::Grid(int numOfColums, int numOfRows, movementType movType)
{
	m_numOfCols = numOfColums;
	m_numOfRows = numOfRows;
	m_movement = movType;
	generatePossibleDirections();
}

//x y can be equal to the width and height which will represent the last point in grid.
/* Return if an element is within a grid. */
inline bool Grid::isWithinGrid(Node node) const
{
	int x = node.getX();
	int y = node.getY();
	return ((x >= 0 && x < m_numOfCols) && (y >= 0 && y < m_numOfRows));
}

/* chekcs if the given node is an obstacle or not.
 * returns false if it's an obstacle true otherwise */
bool Grid::isPassable(Node node) const
{
	unordered_set<Node>::const_iterator it = m_obstacles.find(node);
	//cout << "number of object: " << m_obstacles.count(node) << endl;
	return (m_obstacles.count(node) == 0);
}


/* TODO: convert this to work with multi dimensional grids instead of 2d */
void Grid::generatePossibleDirections()
{
	int size = 0;
	if (m_movement == FOUR_DIRECTIONS) {
		m_directions = { std::make_tuple(1, 0), std::make_tuple(-1, 0), std::make_tuple(0, 1), std::make_tuple(0, -1) };
	}
	else if (m_movement == EIGHT_DIRECTIONS) {
		m_directions = { std::make_tuple(1, 0), std::make_tuple(-1, 0), std::make_tuple(0, 1), std::make_tuple(0, -1), std::make_tuple(1, 1), std::make_tuple(1, -1), std::make_tuple(-1, -1), std::make_tuple(-1, 1) };
	}
}

/* given node, returns its valid neighbors (must not be obstacle, and within the map.) 
TODO: store shared_ptr to Node objects instead
*/
vector<Node> Grid::getNeighbors(Node node) const
{
	vector<Node> results;
	int x = node.getX();
	int y = node.getY();
	int dx, dy = 0;
	
	//range loop instead?
	for (int i = 0; i < m_directions.size(); ++i) {
		tie(dx, dy) = m_directions.at(i);
		Node next = Node(x + dx, y + dy);
		/* insert neighbor node to the vector only if it's passable and within the grid. */
		if (isWithinGrid(next) && isPassable(next)) {
			results.push_back(next);
		}
	}
	return results;
}

Grid::movementType Grid::GetMovementType()
{
	return m_movement;
}