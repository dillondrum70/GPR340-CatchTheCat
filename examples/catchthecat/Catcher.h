#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"

class Catcher: public Agent {
 public:
    explicit Catcher():Agent(){};
    Point2D Move(World*) override;

    std::pair<int, Path> FindHighestPriority(std::vector<Path> optimal, World* world);
};

#endif  // CATCHER_H
