#include "AStar.h"

struct nodeCostCompare
{
	bool operator()(const Node lhs, const Node rhs) const
	{
		return lhs.getPriority() > rhs.getPriority();
	}
};

AStar::AStar(Grid *grid)
{
	m_grid = grid;
}

double AStar::EuclideanHeuristic(Node start, Node goal) const
{
	return sqrt(pow((start.getX() - goal.getX()), 2) + pow((start.getY() - goal.getY()), 2));
}

int AStar::ManhattanHeuristic(Node start, Node goal) const
{
	return abs(start.getX() - goal.getX()) + abs(start.getY() - goal.getY());
}

vector<Node> AStar::search(Node start, Node goal)
{
	if (goal.getX() < 0 || goal.getY() < 0 || goal.getX() >= m_grid->getWidth() || goal.getY() >= m_grid->getHeight())
	  return m_path;
	if (!m_grid->isPassable(goal))
	  return m_path;
	//Priority Queue to reduce number of comparisons in the open list.
	priority_queue<Node, vector<Node>, nodeCostCompare> open;

	/* instead of holding pointers it keeps coppies of the nodes,
	that way the updated cost for nodes wont effect the nodes in the closed set
	*/
	unordered_map<Node, Node> closed; //node is the key, data will be the cost until that node. 
	//closed will be both like cost so far and came from.
	//nodes in closed will have the total cost in nodes
	
	/* Normally the closed set would be empty but this makes the code bit simpler. */
	start.setCost(0); //you're already at start so no cost of movement.
	start.setPriority(0);
	open.push(start);
	closed[start] = start;

	double cost = 0; //path cost from start Node to current Node node.

	//	int loop_counter = 0; //used for debugging purposes only
	/*
	/!((*top) == (*goal))
	* while top of the priority queue is not the goal node:
	* if the open list is empty and we haven't found the node, 
	* then we traversed every accessable node and there is no accessable path from star to end 
	*/
    int count = 0;
	while (!open.empty() && count < 1000) {
		++count;
		//get the top element then REMOVE it from the queue (since we're about to inspect it)
		auto current = open.top();
		open.pop();
		int curx = current.getX(), cury = current.getY();

		//if the current node is the goal, then stop the search since we already found it.
		if (current == goal) {
			break;
		}

		//cout << "Evaluating the new neighbors..." << endl;
		//getneighbors already allocates nodes in heap.
		for (Node next : m_grid->getNeighbors(current)) {
			double cost_diff = (next.getX() - curx == 0 || next.getY() - cury == 0) ? 1 : 1.414;
			cost = closed.at(current).getCost() + cost_diff; //instead of one use current->getCost() for weighted nodes.

			/*If the next node is not in the closed list or the cost is less than the current cost for that node */
			if (!closed.count(next) || cost < closed[next].getCost()) {
				current.setCost(cost);
				closed.emplace(next, current); //put the node in the closed set
				/* adjust the priority of the next node */
				double priority = static_cast<double>(cost) + ((m_grid->GetMovementType() == Grid::movementType::EIGHT_DIRECTIONS) ? EuclideanHeuristic(next, goal) : ManhattanHeuristic(next, goal));
				next.setPriority(priority);
				open.push(next);
			}
		}
	}
	if (count < 1000)
	  reconstructPath(closed, start, goal); //construct the shortest path.
	return m_path.size() > 1 ? std::vector(m_path.begin(), m_path.end() - 1) : m_path;
}

/* Given start, goal, and closed lists, it reconstructs the path (in reverse) and stores it in m_path
* Result will contain the actual shortest path from start to finish
*/
void AStar::reconstructPath(unordered_map<Node, Node> closed, Node start, Node goal)
{
	//vector<Node> path;
	Node current = goal;
	m_path.push_back(current); //incase if start and goal are the same.
	while (current != start) {
		current = closed.at(current);
		m_path.push_back(current);
	}
}
