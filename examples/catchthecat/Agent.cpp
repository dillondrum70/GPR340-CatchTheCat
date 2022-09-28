#include "Agent.h"
#include "World.h"

std::vector<Path> Agent::FindCatShortestPath(World* world) {
	std::cout << "-----------------------------------------------------------\n";
	std::vector<Path> solutions; //first will be shortest, last will be one less than shortest, used for catcher

	bool solutionFound = false;
	Point2D catPos = world->getCat();

	PriorityQueue frontier = PriorityQueue(); //next to explore
	frontier.push(Node(0, catPos));

	WeightMap weights = WeightMap();  // store weights of each node
    weights[frontier.top().point.x][frontier.top().point.y] = 0;

	ParentMap parents = ParentMap();  // store parents of each node

	Path solutionPath = Path();
	while (!frontier.empty() && !solutionFound) 
	{
        Node current = frontier.top();
		frontier.pop();
		Point2D next;

		//std::cout << "\nCurrent: (" << current.point.x << ", " << current.point.y << ")\n";

		for (int i = 0; i < 6 && !solutionFound; i++)
		{
			switch (i)
			{
			case 0:
				//std::cout << "NE : ";
				next = World::NE(current.point);
				break;
			case 1:
				//std::cout << "NW : ";
				next = World::NW(current.point);
				break;
			case 2:
				//std::cout << "W : ";
				next = World::W(current.point);
				break;
			case 3:
				//std::cout << "SW : ";
				next = World::SW(current.point);
				break;
			case 4:
				//std::cout << "SE : ";
				next = World::SE(current.point);
				break;
			case 5:
				//std::cout << "E : ";
				next = World::E(current.point);
				break;
			}
			//std::cout << "Next: (" << next.x << ", " << next.y << ")\n";

			if (world->isValidPosition(next) && !world->getContent(next))
			{
				//if x coord is min x or max x value or if y coord is min y or max y value, return that path
				if (world->catWinsOnSpace(next) && (weights[next.x][next.y] == 0 && next != catPos))
				{
					weights[next.x][next.y] = current.weight + 1;
					parents[next.x][next.y] = current.point;
					//std::cout << "SUCCESS\n\n";
					//only executed once so algorithm time is O(n)
					//the loop that iterates over the 6 directions is constant at 6 so it's complexity is technically O(1)
					solutionPath.push_front(next); //next move cat should take will be at the top after loop, push everything to the front
					Point2D evaluate = current.point;
					
					//trace back through parents to get path, if parent is cat, stop, break, and return path
					while (evaluate != catPos)
					{
						solutionPath.push_front(evaluate);
						evaluate = parents[evaluate.x][evaluate.y];
					}

					//returns after finding all paths that are the same length as the shortest path AND one path that is 1 length greater,
					//the one longer path is used for creating traps once there is only one path of shortest length
					if (solutions.size() > 0 && solutionPath.size() > solutions.front().size())
					{
						solutionFound = true;
					}
					//print solution
					std::cout << catPos << " -> ";
					for (auto const& i : solutionPath)
					{
						std::cout << i << " -> ";
					}
					std::cout << std::endl;

					solutions.push_back(solutionPath);
					solutionPath.clear();
				}
				//if weight of next is null, it hasn't been explored yet, or if it is greater than the new path, we need to explore it
				//and if cat can move to position, position should be explored.  Otherwise, we skip this position
				//if cat wins on this space, it does not need to be explored
				else if (((weights[next.x][next.y] == 0 && next != catPos) || weights[next.x][next.y] > current.weight + 1))
				{
					weights[next.x][next.y] = current.weight + 1;
					parents[next.x][next.y] = current.point;
					frontier.push(Node(weights[next.x][next.y], next));
					//std::cout << "Parent: (" << parents[next.x][next.y].x << ", " << parents[next.x][next.y].y << ")\n";
					//std::cout << "Total Weight: " << weights[next.x][next.y] << "\n\n";
				}
				else
				{
					//std::cout << "Already Explored\n\n";
				}
			}
			else
			{
				//std::cout << "Can not move to (" << next.x << ", " << next.y << ")\n\n";
			}
		}
		PrintMap(world, frontier, weights);
		PrintQueue(frontier);
	}
	//return empty path, no path to exit exists
	return solutions;
}

void Agent::PrintMap(World* world, PriorityQueue pq, WeightMap wm)
{
	for (int y = -world->getWorldSideSize() / 2; y < (world->getWorldSideSize() / 2) + 1; y++)
	{
		if (abs(y) % 2 == 1)
		{
			std::cout << "  ";
		}

		for (int x = -world->getWorldSideSize() / 2; x < (world->getWorldSideSize() / 2) + 1; x++)
		{
			Point2D current = Point2D(x, y);

			if (world->getCat() == current)
			{
				std::cout << "C   ";
			}
			else if (current == Point2D())
			{
				std::cout << "Z   ";
			}
			else if (world->getContent(current))
			{
				std::cout << "#   ";
			}
			else
			{
				bool inFrontier = false;
				PriorityQueue temp = pq;
				for (int i = temp.size() - 1; i >= 0 ; i--)
				{
					if (temp.top().point == current)
					{
						std::cout << "1   "; //if in frontier, shows 1
						inFrontier = true;
						break;
					}
					temp.pop();
				}
				if (!inFrontier)
				{
					if (wm[x][y] != 0)
					{
						std::cout << "2   ";
					}
					else
					{
						std::cout << "0   ";
					}
				}
			}
		}
		std::cout << std::endl;
	}
}

void Agent::PrintQueue(PriorityQueue pq)
{
	for (int i = 0; i < pq.size(); i++)
	{
		std::cout << pq.top().weight << " - (" << pq.top().point.x << ", " << pq.top().point.y << ")\n";
		pq.pop();
	}
}