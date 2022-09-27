#ifndef AGENT_H
#define AGENT_H

#include <list>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_map>

#include "Point2D.h"

class World;

typedef int Weight;

struct Node
{
    Weight weight = 0;
    Point2D point = Point2D();

    Node() {}
    Node(Weight vWeight, Point2D vPoint) { weight = vWeight; point = vPoint; }

    bool operator<(const Node& rhs) const { return weight < rhs.weight; }
    bool operator<=(const Node& rhs) const { return weight <= rhs.weight; }
    bool operator>(const Node& rhs) const { return weight > rhs.weight; }
    bool operator==(const Node& rhs) const { return (weight == rhs.weight && point == rhs.point); }
    bool operator!=(const Node& rhs) const { return (weight != rhs.weight || point != rhs.point); }
};

typedef std::list<Point2D> Path;
typedef std::unordered_map<int, std::unordered_map<int, Point2D>> ParentMap;
typedef std::unordered_map<int, std::unordered_map<int, int>> WeightMap;
typedef std::priority_queue<Node, std::vector<Node>, std::greater<Node>> PriorityQueue;

class Agent {
public:
  explicit Agent()= default;;
  virtual Point2D Move(World*)=0;

  //returns empty Path() if no winning path exists, returns shortest path otherwise
  std::list<Path> FindCatShortestPath(World* world);

  void PrintMap(World* world, PriorityQueue pq, WeightMap wm);
  void PrintQueue(PriorityQueue pq);
};

#endif  // AGENT_H
