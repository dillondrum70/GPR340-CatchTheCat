#ifndef AGENT_H
#define AGENT_H
#include "Point2D.h"

#include <list>

class World;

typedef std::list<Point2D> Path;

class Agent {
public:
  explicit Agent()= default;;
  virtual Point2D Move(World*)=0;

  Path FindCatShortestPath(Point2D catPos);
};

#endif  // AGENT_H
