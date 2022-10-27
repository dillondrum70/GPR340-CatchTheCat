#include "MazeGenerator.h"
#include "World.h"
#include "Engine.h"

#include <ctime>
#include <algorithm>

void MazeGenerator::GenerateMaze(World* world)
{
	switch (currentAlgorithm)
	{
	case RECURSIVE_BACKTRACK:
		RecursiveBacktrack(world);
		break;
	case PRIM:
		Prim(world);
		break;
	default:
		throw("Algorithm Does Not Exist");
	}
}

void MazeGenerator::InitRecursiveBacktrack(World* world)
{
	int sideSize = world->GetSize();

	visited = std::vector<bool>(sideSize * sideSize, false);
	visited[0] = true;

	random = std::mt19937(std::time(nullptr));
	stack.clear();
	stack.push_front(std::make_pair(Point2D(-sideSize / 2, -sideSize / 2), directions));

	inProcess = true;
}

void MazeGenerator::RecursiveBacktrack(World* world) {
  // todo: use getnode or setnode to navigate over the world.
	int sideSize = world->GetSize();

	if (!inProcess)
	{
		InitRecursiveBacktrack(world);
	}

	while (!stack.empty())
	{
		RecursiveStep(world);

		//PrintMap(world);
	}

	inProcess = false;
}

void MazeGenerator::RecursiveStep(World* pWorld)
{
	int sideSize = pWorld->GetSize();
	int sizeOver2 = sideSize / 2;

	if (!inProcess)
	{
		InitRecursiveBacktrack(pWorld);
	}

	if (!stack.empty())
	{
		std::pair<Point2D, std::vector<int>> current = stack.front();

		while (current.second.size() > 0)
		{
			//Get random direction
			int randIndex = random() % current.second.size();
			int dir = current.second[randIndex];
			current.second.erase(current.second.begin() + randIndex);

			const Point2D newPoint(current.first.x + dx[dir], current.first.y + dy[dir]);

			if (newPoint.x >= -sizeOver2 && newPoint.x < sizeOver2 + 1 &&
				newPoint.y >= -sizeOver2 && newPoint.y < sizeOver2 + 1 &&
				!visited[((newPoint.y + sizeOver2) * sideSize) + (newPoint.x + sizeOver2)])
			{
				visited[((newPoint.y + sizeOver2) * sideSize) + (newPoint.x + sizeOver2)] = true;

				switch (dir)
				{
				case 0:
					pWorld->SetNorth(current.first, false);
					stack.push_front(std::make_pair(newPoint, directions));
					//CarvePath(newPoint, vSideSize, pWorld);
					break;
				case 1:
					pWorld->SetEast(current.first, false);
					stack.push_front(std::make_pair(newPoint, directions));
					//CarvePath(newPoint, vSideSize, pWorld);
					break;
				case 2:
					pWorld->SetSouth(current.first, false);
					stack.push_front(std::make_pair(newPoint, directions));
					//CarvePath(newPoint, vSideSize, pWorld);
					break;
				case 3:
					pWorld->SetWest(current.first, false);
					stack.push_front(std::make_pair(newPoint, directions));
					//CarvePath(newPoint, vSideSize, pWorld);
					break;
				default:
					throw("Out of Range");
				}

				//Stop after you reach the first successful direction
				break;
			}
		}

		if (current.second.size() <= 0)
		{
			stack.pop_front();
			return;
		}
	}
	else
	{
		inProcess = false;
	}
}

void MazeGenerator::InitPrim(World* world)
{
	int sideSize = world->GetSize();
	int sizeOver2 = sideSize / 2;

	inProcess = true;
	random = std::mt19937(std::time(nullptr));
	visited.clear();
	visited = std::vector<bool>(sideSize * sideSize, false);

	Point2D start(random() % sideSize - sizeOver2, random() % sideSize - sizeOver2);
	frontier.clear();

	frontier.push_back(start);
}

void MazeGenerator::Prim(World* world)
{
	int sideSize = world->GetSize();
	int sizeOver2 = sideSize / 2;

	if (!inProcess)
	{
		InitPrim(world);
	}

	while (!frontier.empty())
	{
		PrimStep(world);
	}

	inProcess = false;
}

void MazeGenerator::PrimStep(World* world)
{
	int sideSize = world->GetSize();
	int sizeOver2 = sideSize / 2;

	if (!inProcess)
	{
		InitPrim(world);
	}

	if (!frontier.empty())
	{

		//Get current point from frontier randomly
		int randIndex = random() % frontier.size();
		Point2D current = frontier[randIndex];
		frontier.erase(frontier.begin() + randIndex);

		if (visited[((current.y + sizeOver2) * sideSize) + (current.x + sizeOver2)])
		{
			return;
		}

		visited[((current.y + sizeOver2) * sideSize) + (current.x + sizeOver2)] = true;

		std::vector<int> nextCarves; //vector of directions that could be carved to from current

		//Add neighbors to proper collections if they are valid
		for (int i = 0; i < 4; i++)
		{
			Point2D next = Point2D(current.x + dx[i], current.y + dy[i]);

			if (next.x >= -sizeOver2 && next.x < sizeOver2 + 1 &&
				next.y >= -sizeOver2 && next.y < sizeOver2 + 1)
			{
				if (!visited[((next.y + sizeOver2) * sideSize) + (next.x + sizeOver2)])
				{
					//if not visited, add to frontier
					frontier.push_back(next);
				}
				else
				{
					//if visited, log as possible wall to carve out
					nextCarves.push_back(i);
				}
			}
		}

		//randomly choose which wall to carve
		if (nextCarves.size() > 0)
		{
			switch (nextCarves[random() % nextCarves.size()])
			{
			case 0:
				world->SetNorth(current, false);
				break;
			case 1:
				world->SetEast(current, false);
				break;
			case 2:
				world->SetSouth(current, false);
				break;
			case 3:
				world->SetWest(current, false);
				break;
			default:
				throw("Out of Range");
			}
		}

		//PrintMap(world);
	}
	else
	{
		inProcess = false;
	}
}

/*std::vector<int> MazeGenerator::ShuffleDirections()
{
	std::mt19937 random(std::time(nullptr)); //initialize mersenne twister with seed

	std::vector<int> shuffled;
	std::vector<int> directions{ 0, 1, 2, 3 };

	for (int i = 0; i < 4; i++)
	{
		int index = random() % (4 - i);
		shuffled.push_back(directions[index]);
		directions.erase(directions.begin() + index);
	}

	return shuffled;
}*/

void MazeGenerator::PrintMap(World* pWorld)
{
	system("cls");
	//Y
	for (int i = 0; i < pWorld->GetSize(); i++)
	{
		//X
		//horizontal lines
		for (int j = 0; j < pWorld->GetSize(); j++)
		{
			std::cout << ".";
			if (pWorld->GetNorth(Point2D(j, i)))
			{
				std::cout << "-";
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << "." << std::endl;

		//vertical lines
		for (int k = 0; k < pWorld->GetSize(); k++)
		{
			if (pWorld->GetWest(Point2D(k, i)))
			{
				std::cout << "|";
			}
			else
			{
				std::cout << " ";
			}
			std::cout << " ";
		}
		//print far right wall
		if (pWorld->GetEast(Point2D(pWorld->GetSize() - 1, i)))
		{
			std::cout << "|";
		}
		else
		{
			std::cout << " ";
		}
		std::cout << std::endl;
	}

	//print far bottom wall
	for (int j = 0; j < pWorld->GetSize(); j++)
	{
		std::cout << ".";
		if (pWorld->GetSouth(Point2D(j, pWorld->GetSize() - 1)))
		{
			std::cout << "-";
		}
		else
		{
			std::cout << " ";
		}
	}
	std::cout << "." << std::endl;
}
