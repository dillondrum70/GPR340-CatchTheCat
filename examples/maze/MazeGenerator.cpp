#include "MazeGenerator.h"
#include "World.h"

#include <ctime>
#include <random>

void MazeGenerator::Generate(World* world) {
  // todo: use getnode or setnode to navigate over the world.

	int sideSize = world->GetSize();

	visited = new bool[sideSize * sideSize]; 
	
	CarvePath(Point2D(0, 0), sideSize, world);
}

void MazeGenerator::CarvePath(const Point2D& vPoint, int vSideSize, World* pWorld)
{
	//PrintMap(pWorld);

	std::vector<int> directions = ShuffleDirections();
	
	for (int dir : directions)
	{
		const Point2D newPoint(vPoint.x + dx[dir], vPoint.y + dy[dir]);

		if (newPoint.x >= 0 && newPoint.x < vSideSize &&
			newPoint.y >= 0 && newPoint.y < vSideSize &&
			!visited[(newPoint.y * vSideSize) + newPoint.x])
		{
			visited[(newPoint.y * vSideSize) + newPoint.x] = true;

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

std::vector<int> MazeGenerator::ShuffleDirections()
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
}

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
