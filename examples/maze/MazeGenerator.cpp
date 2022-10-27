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

void MazeGenerator::RecursiveBacktrack(World* world) {
  // todo: use getnode or setnode to navigate over the world.

	int sideSize = world->GetSize();

	visited = std::vector<bool>(sideSize * sideSize, false);
	visited[0] = true;

	random = std::mt19937(std::time(nullptr));
	CarvePath(Point2D(-sideSize / 2, -sideSize / 2), sideSize, world);

	sideSize = 0;
}

void MazeGenerator::CarvePath(const Point2D& vPoint, int vSideSize, World* pWorld)
{
	int sizeOver2 = vSideSize / 2;
	//PrintMap(pWorld);

	//std::vector<int> directions = ShuffleDirections();
	
	std::shuffle(&directions[0], &directions[4], random);
	
	for (int dir : directions)
	{
		const Point2D newPoint(vPoint.x + dx[dir], vPoint.y + dy[dir]);

		if (newPoint.x >= -sizeOver2 && newPoint.x < sizeOver2 + 1 &&
			newPoint.y >= -sizeOver2 && newPoint.y < sizeOver2 + 1 &&
			!visited[((newPoint.y + sizeOver2) * vSideSize) + (newPoint.x + sizeOver2)])
		{
			visited[((newPoint.y + sizeOver2) * vSideSize) + (newPoint.x + sizeOver2)] = true;

			switch (dir)
			{
			case 0:
				pWorld->SetNorth(vPoint, false);
				CarvePath(newPoint, vSideSize, pWorld);
				break;
			case 1:
				pWorld->SetEast(vPoint, false);
				CarvePath(newPoint, vSideSize, pWorld);
				break;
			case 2:
				pWorld->SetSouth(vPoint, false);
				CarvePath(newPoint, vSideSize, pWorld);
				break;
			case 3:
				pWorld->SetWest(vPoint, false);
				CarvePath(newPoint, vSideSize, pWorld);
				break;
			default:
				throw("Out of Range");
			}
		}
	}
}

void MazeGenerator::Prim(World* world)
{
	int sideSize = world->GetSize();
	int sizeOver2 = sideSize / 2;

	if (!inProcess)
	{
		inProcess = true;
		random = std::mt19937(std::time(nullptr));
		visited.clear();
		visited = std::vector<bool>(sideSize * sideSize, false);

		Point2D start(random() % sideSize - sizeOver2, random() % sideSize - sizeOver2);
		frontier.clear();

		frontier.push_back(start);
	}
	

	while (!frontier.empty())
	{
		
		/*//Get current point from frontier randomly
		int randIndex = random() % frontier.size();
		Point2D current = frontier[randIndex];
		frontier.erase(frontier.begin() + randIndex);

		if (visited[((current.y + sizeOver2) * sideSize) + (current.x + sizeOver2)])
		{
			continue;
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

		//PrintMap(world);*/
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
		inProcess = true;
		random = std::mt19937(std::time(nullptr));
		visited.clear();
		visited = std::vector<bool>(sideSize * sideSize, false);

		Point2D start(random() % sideSize - sizeOver2, random() % sideSize - sizeOver2);
		frontier.clear();

		frontier.push_back(start);
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
