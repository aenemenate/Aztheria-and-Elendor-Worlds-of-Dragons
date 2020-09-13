#include "../../../include/AStar/Node.h"



/* SPECIALIZED STD NOT GETTING CALLED!!!! */
/* Hash function for Node class, which will be used for std::unordered_set */

Node::Node()
{
	X = 0;
	Y = 0;
	m_cost = 1;
	m_priority = 0;
}

 bool Node::operator==(Node const &rhs) const
{
	return (X == rhs.getX() && Y == rhs.getY());
}

 bool Node::operator!=(Node const &rhs) const
 {
	 return (X != rhs.getX() || Y != rhs.getY());
 }


 /* default constructor, if cost is not defined it'll be 1 */
 Node::Node(int x, int y)
 {
	 this->X = x;
	 this->Y = y;
	 m_cost = 1;
 }

 //for nodes with weights.
 Node::Node(int x, int y, int cost)
 {
	 this->X = x;
	 this->Y = y;
	 m_cost = cost;
	 m_priority = 0;
 }


 //setter and getter for priority which will be used by the priority queue.

 double Node::getPriority() const
 {
	 return m_priority;
 }
 void Node::setPriority(double priority)
 {
	 m_priority = priority;
 }


/* Path Cost*/
int Node::getCost() const
{
	return m_cost;
}

void Node::setCost(int cost)
{
	m_cost = cost;
}


Node::~Node()
{
}


int Node::getX() const
{
	return X;
}
int Node::getY() const
{
	return Y;
}
