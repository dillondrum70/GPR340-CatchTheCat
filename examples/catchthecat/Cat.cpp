#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
    
    if (!noEscape)
    {
        std::vector<Path> optimal = FindCatShortestPath(world);
        if (optimal.size() > 0)
        {

            return optimal[0].front(); //first position (next position) of the first path (the shortest)
        }
        
        noEscape = true;
    }

    //Won't be executed if there is an escape for the cat, it would return first
    auto pos = world->getCat();
    int sideOver2 = world->getWorldSideSize() / 2;

    //all adjacent positions
    std::vector<int> adjacentPositions = { 0, 1, 2, 3, 4, 5 };

    //loop through adjacent positions and randomly choose the first valid position
    for (int i = 0; i < 6; i++)
    {
        //choose random position
        int rand = Random::Range(0, adjacentPositions.size() - 1);

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
        adjacentPositions.erase(adjacentPositions.begin() + rand);
    }

    return pos; //return cat position, trapped, no escape if all sides are closed off
}
