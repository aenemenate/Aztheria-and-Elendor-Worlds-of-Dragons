#include "AStar.h"

/* Private structures used as comparator functions for Nodes */
struct nodeCostCompare
{
	bool operator()(const Node &lhs, const Node &rhs) const
	{
		return lhs.getCost() > rhs.getCost();
	}
};


struct ptrNodeCostCompare
{
	bool operator()(const Node lhs, const Node rhs) const
	{
		return lhs.getPriority() > rhs.getPriority();
	}
};

/*Grid obj must be allocated in the calling class. */
AStar::AStar(Grid *grid)
{
	m_grid = grid;

}

/* it might be useful when multi-dimensional maps involved, but requires more research
* inlined
*/
double AStar::EuclideanHeuristic(Node start, Node goal) const
{
	return sqrt(pow((start.getX() - goal.getX()), 2) + pow((start.getY() - goal.getY()), 2));
}


/* Use it when Manhattan movement is allowed 
 * inlined
*/
int AStar::ManhattanHeuristic(Node start, Node goal) const
{
	return abs(start.getX() - goal.getX()) + abs(start.getY() - goal.getY());
}

/* Use it when Diagonal movement is allowed 
 * inlined
*/
int AStar::DiagonalHeuristic(Node start, Node goal) const
{
	return max(abs(start.getX() - goal.getX()), abs(start.getY() - goal.getY()));
}

/* Main search function that finds the shortest path from start to goal node
 * Detailed description of how it works explained within the function. 
 * Returns a vector containing nodes constructing the shortest path from start to goal (including start and goal)
 * 
 * TODO: Currently manhattanDistance heuristic is hardcoded, depending on grid's movement type, return a function pointer instead.
 * http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
 *  Also better memory management!, Use unique_ptrs instead.
 */
vector<Node> AStar::search(Node start, Node goal)
{
	if (goal.getX() < 0 || goal.getY() < 0 || goal.getX() >= m_grid->getWidth() || goal.getY() >= m_grid->getHeight())
	  return m_path;
	if (!m_grid->isPassable(goal))
	  return m_path;
	//Priority Queue to reduce number of comparisons in the open list.
	priority_queue<Node, vector<Node>, ptrNodeCostCompare> open;

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

	int cost = 0; //path cost from start Node to current Node node.

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

		//if the current node is the goal, then stop the search since we already found it.
		if (current == goal) {
			break;
		}

		//cout << "Evaluating the new neighbors..." << endl;
		//getneighbors already allocates nodes in heap.
		for (auto next : m_grid->getNeighbors(current)) {
			cost = closed.at(current).getCost() + 1; //instead of one use current->getCost() for weighted nodes.

			/*If the next node is not in the closed list or the cost is less than the current cost for that node */
			if (!closed.count(next) || cost < closed[next].getCost()) {
				current.setCost(cost); //edit the cost of closed to be the total cost from that node.
				closed.emplace(next, current); //put the node in the closed set
				
				/* adjust the priority of the next node */
				double priority = static_cast<double>(cost) + EuclideanHeuristic(next, goal);
				next.setPriority(priority);
				open.push(next);
			}
		}
	}
	if (count < 1000)
	  reconstructPath(closed, start, goal); //construct the shortest path.
	return std::vector(m_path.begin(), m_path.end() - 1);
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