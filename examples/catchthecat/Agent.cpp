#include "Agent.h"
#include "World.h"

Path Agent::FindCatShortestPath(World* world) {

	bool solutionFound = false;
	Point2D catPos = world->getCat();

	PriorityQueue frontier = PriorityQueue(); //next to explore
	frontier.push(std::make_pair(0, catPos));

	WeightMap weights = WeightMap();  // store weights of each node
    weights[frontier.top().second.x][frontier.top().second.y] = 0;

	ParentMap parents = ParentMap();  // store parents of each node

	Path solution = Path();
	while (!frontier.empty() && !solutionFound) 
	{
        Point2D current = frontier.top().second;
		frontier.pop();
		Point2D next;

		std::cout << "\nCurrent: (" << current.x << ", " << current.y << ")\n";

		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:
				std::cout << "NE : ";
				next = World::NE(current);
				break;
			case 1:
				std::cout << "NW : ";
				next = World::NW(current);
				break;
			case 2:
				std::cout << "W : ";
				next = World::W(current);
				break;
			case 3:
				std::cout << "SW : ";
				next = World::SW(current);
				break;
			case 4:
				std::cout << "SE : ";
				next = World::SE(current);
				break;
			case 5:
				std::cout << "E : ";
				next = World::E(current);
				break;
			}
			std::cout << "Next: (" << next.x << ", " << next.y << ")\n";

			if (world->isValidPosition(next) && !world->getContent(next))
			{
				//if x coord is min x or max x value or if y coord is min y or max y value, return that path
				if (world->catWinsOnSpace(next))
				{
					std::cout << "SUCCESS\n\n";
					//only executed once so algorithm time is O(n)
					//the loop that iterates over the 6 directions is constant at 6 so it's complexity is technically O(1)
					solution.push_front(next); //next move cat should take will be at the top after loop, push everything to the front
					Point2D evaluate = current;
					
					//trace back through parents to get path, if parent is cat, stop, break, and return path
					while (evaluate != catPos)
					{
						solution.push_front(evaluate);
						evaluate = parents[evaluate.x][evaluate.y];
					}
					solutionFound = true;
				}

				//if weight of next is null, it hasn't been explored yet, or if it is greater than the new path, we need to explore it
				//and if cat can move to position, position should be explored.  Otherwise, we skip this position
				if (((weights[next.x][next.y] == 0 && next != catPos) || weights[next.x][next.y] > weights[current.x][current.y] + 1))
				{
					weights[next.x][next.y] = weights[current.x][current.y] + 1;
					parents[next.x][next.y] = current;
					frontier.push(std::make_pair(weights[next.x][next.y], next));
					std::cout << "Parent: (" << parents[next.x][next.y].x << ", " << parents[next.x][next.y].y << ")\n";
					std::cout << "Total Weight: " << weights[next.x][next.y] << "\n\n";
				}
				else
				{
					std::cout << "Already Explored\n\n";
				}
			}
			else
			{
				std::cout << "Can not move to (" << next.x << ", " << next.y << ")\n\n";
			}
		}
	}
	//return empty path, no path to exit exists
	return solution;
}