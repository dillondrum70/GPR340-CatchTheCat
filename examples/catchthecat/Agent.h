#ifndef AGENT_H
#define AGENT_H

#include <list>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_map>

#include "Point2D.h"

class World;

typedef std::list<Point2D> Path;
typedef int Weight;
typedef std::unordered_map<int, std::unordered_map<int, Point2D>> ParentMap;
typedef std::unordered_map<int, std::unordered_map<int, int>> WeightMap;
typedef std::priority_queue<std::pair<Weight, Point2D>, std::vector<std::pair<Weight, Point2D>>, std::less<std::pair<Weight, Point2D>>> PriorityQueue;

class Agent {
public:
  explicit Agent()= default;;
  virtual Point2D Move(World*)=0;

  //returns empty Path() if no winning path exists, returns shortest path otherwise
  Path FindCatShortestPath(World* world);
};

#endif  // AGENT_H
