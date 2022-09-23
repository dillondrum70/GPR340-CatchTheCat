#include "Agent.h"
#include "World.h"

#include <unordered_map>

Path Agent::FindCatShortestPath(World* world) {

	PriorityQueue frontier = PriorityQueue(); //next to explore
	frontier.push(std::make_pair(0, world->getCat()));

	std::unordered_map<Point2D, Weight> weights = std::unordered_map<Point2D, Weight>();//store weights of each node
	weights[frontier.top().second] = 0;

	std::unordered_map<Point2D, Point2D> parents = std::unordered_map<Point2D, Point2D>();//store parents of each node

	bool pathFound = false;

	while (!frontier.empty()) 
	{
        Point2D current = frontier.top().second;

		for (int i = 0; i < 6; i++) 
		{
			Point2D next = Point2D(0, 0);
              
			switch (i) 
			{
				case 0:
					next = World::NE(next);
					break;
				case 1:
					next = World::NW(next);
					break;
				case 2:
					next = World::W(next);
					break;
				case 3:
					next = World::SW(next);
					break;
				case 4:
					next = World::SE(next);
					break;
				case 5:
					next = World::E(next);
					break;
			}

			//if x coord is 0 or max x value or if y coord is 0 or max y value, return that path
			if (!world->isValidPosition(next))
			{
				pathFound = true;
				break;//return path to next
			}

			//TODO: check that node hasn't been explored yet

			if (world->catCanMoveToPosition(next))
			{
				weights[next] = weights[current] + 1;
				parents[next] = current;
				frontier.push(std::make_pair(weights[next], next));
			}
		}

		frontier.pop();
	}

	if (pathFound)
	{
		//TODO: return path through parents
	}
	else
	{
		//return empty path, no path to exit exists
		return Path();
	}
}