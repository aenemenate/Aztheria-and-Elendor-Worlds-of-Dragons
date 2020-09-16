#pragma once

#include <vector>
class Node
{
	std::vector<int> dimensions;
	int X, Y;
	int m_cost;
	double m_priority; // this is purely for the priority queue to keep 
// TODO: Make this to accomadete N-dimensions by using templates
public:
	Node(int x, int y, int cost);
	Node(int x, int y);
	Node();

	//get rid of these later.
	int getX() const;
	int getY() const;
	int getCost() const;
	double getPriority() const;

	void setPriority(double priority);
	void setCost(int cost);

	bool operator==(Node const &rhs) const;
	bool operator!=(Node const &rhs) const;
};


/* works just like boost's hash_combine */
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}



/* Template specialisation to accomodate Node type in cpp containters such as unoredered_set */
namespace std
{
	template<>
	struct hash<Node>
	{
		std::size_t operator()(const Node &node) const
		{
			//starts with hash value 0 of 0 will be modified by XORing and bit-shifting in hash_combine function.
			size_t seed = 0;
			hash_combine(seed, node.getX());
			hash_combine(seed, node.getY());
			return seed;
		}
	};
}

