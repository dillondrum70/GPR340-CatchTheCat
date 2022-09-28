#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"

class Catcher: public Agent {
private:
    bool noEscape = false;

 public:
    explicit Catcher():Agent(){};
    Point2D Move(World*) override;
    void Reset() { noEscape = false; }
    std::pair<int, Path> FindHighestPriority(std::vector<Path> optimal, World* world);
};

#endif  // CATCHER_H
