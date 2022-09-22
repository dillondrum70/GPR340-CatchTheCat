#include "Agent.h"

#include <queue>

Path Agent::FindCatShortestPath(Point2D catPos) 
{
	std::priority_queue<int, Point2D, std::greater<int>> frontier;
	//maybe use list of pair<cost, point>?

	frontier.push(catPos);

	while (!frontier.empty()) 
	{
		//if x coord is 0 or max x value or if y coord is 0 or max y value, return that path
	}
}