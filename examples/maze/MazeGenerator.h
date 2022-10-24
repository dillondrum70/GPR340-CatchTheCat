#ifndef MOBAGEN_MAZEGENERATOR_H
#define MOBAGEN_MAZEGENERATOR_H
#include "Point2D.h"

#include <vector>
#include <random>

class World;

class MazeGenerator {
private:
	//N = 0, E = 1, S = 2, W = 3
	const int dx[4] = { 0, 1, 0, -1 };
	const int dy[4] = { -1, 0, 1, 0 }; //0, 0 is in the top right corner, North is negative
	bool* visited = nullptr;//1d array representing visited grid spaces
	std::mt19937 random;

	void CarvePath(const Point2D& vPoint, int vSideSize, World* world);

	std::vector<int> ShuffleDirections();

	void PrintMap(World* pWorld);
 public:
  // todo: in order to step work properly, you have to store your current
  // exploration status in the MazeGenerator members
  void Generate(World * world);
};

#endif
