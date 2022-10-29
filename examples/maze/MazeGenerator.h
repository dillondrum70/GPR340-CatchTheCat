#ifndef MOBAGEN_MAZEGENERATOR_H
#define MOBAGEN_MAZEGENERATOR_H
#include "Point2D.h"

#include <vector>
#include <random>
#include <list>
#include <utility>

class World;
class Engine;

enum Algorithms
{
	RECURSIVE_BACKTRACK = 0,
	PRIM,
	TOTAL_ALGORITHMS
};

class MazeGenerator {
private:
	//N = 0, E = 1, S = 2, W = 3
	std::vector<int> directions = { 0, 1, 2, 3 };
	const int dx[4] = { 0, 1, 0, -1 };
	const int dy[4] = { -1, 0, 1, 0 }; //0, 0 is in the top right corner, North is negative
	std::vector<bool> visited;//1d array representing visited grid spaces
	std::mt19937 random;	//Random number generator

	//Recursive
	std::list<std::pair<Point2D, std::vector<int>>> stack;	//Stores stack of points
	//std::vector<std::vector<int>> exploredDirections; //Stores which directions have been explored at each point

	//Prim
	std::vector<Point2D> frontier;

	int currentAlgorithm = RECURSIVE_BACKTRACK;

	bool inProcess = false;	//True if an algorithm is currently running
	

	std::vector<int> ShuffleDirections();

	void PrintMap(World* pWorld);
 public:
	 bool GetInProcess() { return inProcess; }
	 int GetAlgorithm() { return currentAlgorithm; }
	 std::vector<Point2D> GetFrontier() { return frontier; }
	 std::list<std::pair<Point2D, std::vector<int>>> GetStack() { return stack; }

	 //% total: ensures it is in the range of accepted algorithms
	 void SetAlgorithm(int vAlgorithm) { currentAlgorithm = vAlgorithm % TOTAL_ALGORITHMS; }

  // todo: in order to step work properly, you have to store your current
  // exploration status in the MazeGenerator members
	 void GenerateMaze(World* world);
	 void StepMaze(World* world);

	 void InitRecursiveBacktrack(World* world);
	void RecursiveBacktrack(World * world);
	void RecursiveStep(World* world);

	void InitPrim(World* world);
	void Prim(World* world);
	void PrimStep(World* world);
};

#endif
