#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  /*auto side = world->getWorldSideSize() / 2;
  for(;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    auto cat = world->getCat();
    if(cat.x!=p.x && cat.y!=p.y && !world->getContent(p))
      return p;
  }*/

    if (!noEscape)
    {
        std::list<Path> optimal = FindCatShortestPath(world);

        if (optimal.size() > 0)
        {
            return optimal.front().back(); //last position of the first path (the shortest)
        }
        
        noEscape = true;
    }

    auto pos = world->getCat();

    //all adjacent positions
    std::vector<int> adjacentPositions = { 0, 1, 2, 3, 4, 5 };

    //loop through adjacent positions and randomly choose the first valid position
    for (int i = 0; i < adjacentPositions.size(); i++)
    {
        //choose random position
        auto rand = Random::Range(0, adjacentPositions.size() - 1);

        switch (adjacentPositions[rand]) {
        case 0:
            if (world->catCanMoveToPosition(World::NE(pos)))
                return World::NE(pos);
            break;
        case 1:
            if (world->catCanMoveToPosition(World::NW(pos)))
                return World::NW(pos);
            break;
        case 2:
            if (world->catCanMoveToPosition(World::E(pos)))
                return World::E(pos);
            break;
        case 3:
            if (world->catCanMoveToPosition(World::W(pos)))
                return World::W(pos);
            break;
        case 4:
            if (world->catCanMoveToPosition(World::SW(pos)))
                return World::SW(pos);
            break;
        case 5:
            if (world->catCanMoveToPosition(World::SE(pos)))
                return World::SE(pos);
            break;
        default:
            throw "random out of range";
        }

        //erase this position from list of valid adjacent positions
        adjacentPositions.erase(adjacentPositions.begin(), adjacentPositions.end() + rand);
    }

    return pos; //return cat position, trapped, should never happen, if cat is trapped, it should know that before executing Catcher.Move()
}
