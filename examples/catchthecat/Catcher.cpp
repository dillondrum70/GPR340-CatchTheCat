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
    int sideOver2 = world->getWorldSideSize() / 2;
    Point2D cat = world->getCat();

    if (!noEscape)
    {
        std::vector<Path> optimal = FindCatShortestPath(world);
        //std::cout << optimal.size() << std::endl;

        //return normal optimal path if the cat is adjacent to an exit space
        if (optimal.size() > 0 && world->catWinsOnSpace(optimal[0].back()) && world->catCanMoveToPosition(optimal[0].back()))
        {
            //std::cout << "Adjacent To Exit\n";
            return optimal[0].back(); //last position of the first path (the shortest)
        }

        //block second shortest path if there is only the shortest path and the path that is one longer than the shortest
        if (optimal.size() == 2 && optimal[0].size() != optimal[1].size()) 
        {
            //std::cout << "Two Unequal Paths\n";
            return optimal[1].back();
        }

        //do highest priority check based on number of adjacent walls
        if (optimal.size() >= 2)
        {
            //std::cout << "Highest Priority\n";
            //return optimal[1].back(); //last position of the second path (the second shortest)
            return FindHighestPriority(optimal, world).second.back(); //last position of the path with the highest priority
        }

        //catch in case the last ones fail and there is still a shortest path
        if(optimal.size() > 0) 
        {
            //std::cout << optimal[0].size() << " and " << optimal[1].size() << std::endl;

            //print paths
            /*for (int j = 0; j < optimal.size(); j++)
            {
                for (auto const& i : optimal[j])
                {
                    std::cout << i << " -> ";
                }
                std::cout << std::endl;
            }*/

            return optimal[0].back(); //last position of the first path (the shortest)
        }

        noEscape = true;
    }

    auto pos = world->getCat();

    //all adjacent positions
    std::vector<int> adjacentPositions = { 0, 1, 2, 3, 4, 5 };

    //loop through adjacent positions and randomly choose the first valid position
    for (int i = 0; i < 6; i++)
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
        adjacentPositions.erase(adjacentPositions.begin() + rand);
    }

    return pos; //return cat position, trapped, should never happen, if cat is trapped, it should know that before executing Catcher.Move()
}

std::pair<int, Path> Catcher::FindHighestPriority(std::vector<Path> optimal, World* world)
{
    int sideOver2 = world->getWorldSideSize() / 2;

    //DONE: Ignore top left and bottom left corners
    //DONE: Prioritize paths not adjacent to existing walls
    //TODO: Special case for top right and bottom right corners, getting to (sideOver2 - 1, sideOver2 - 1) or (sideOver2 - 1, -sideOver2 + 1)
        //means that there may be up to 3 exits adjacent, may be different spaces depending on which side the cat approaches from
    //TODO: Handle cases where wall blocks access to an exit space in a way that you can only reach that exit space through other exit spaces,
        //shouldn't waste time blocking off that blocked exit space
        //May already be handled indirectly with priority system
    //Point2D topLeft = Point2D(-sideOver2, -sideOver2);
    //Point2D bottomLeft = Point2D(-sideOver2, sideOver2);

    //stores shortest path with least amount of adjacent walls
        //7 should be impossible, placeholder so first valid path checked will always be made priority
    std::pair<int, Path> priority = std::make_pair(-1, Path());

    for (int i = optimal.size() - 1; i >= 0; i--)
    {
        //if path is the top or bottom left, it can be ignored because the cat would reach the exit by standing on 
        //either space needed to reach those corners
        //if (optimal[i].back() == topLeft || optimal[i].back() == bottomLeft)
        //{
            //optimal.erase(optimal.begin() + i);
            //skip
        //}
        /*else*/ 
        if (optimal[i].size() == optimal[0].size()) //if this path length is the same as the shortest, figure out if it is a priority
        {
            //prioritize paths that aren't adjacent to existing exit walls, creates traps
            Point2D back = optimal[i].back();
            bool xMax = abs(back.x) == sideOver2;
            bool yMax = abs(back.y) == sideOver2;

            int p = 0;

            //sum adjacent spaces that are devoid of walls
            if (xMax && !yMax)
            {
                //check space above, changes between northeast and northwest, easier to just subtract or add 1 to y for this case
                if (!world->getContent(Point2D(back.x, back.y - 1)))
                {
                    p++;
                }
                if (!world->getContent(Point2D(back.x, back.y + 1)))
                {
                    p++;
                }

                if (back.x > 0 && !world->getContent(Point2D(back.x - 1, back.y))) //if on right side of map, check left
                {
                    p++;
                }
                else if (back.x < 0 && !world->getContent(Point2D(back.x + 1, back.y))) //if on left side of map, check right
                {
                    p++;
                }
            }
            else if (yMax && !xMax)
            {
                //check east and west
                if (!world->getContent(World::W(back)))
                {
                    p++;
                }
                if (!world->getContent(World::E(back)))
                {
                    p++;
                }

                if (back.y > 0) //if on lower side of map, northeast and northwest
                {
                    if (!world->getContent(World::NE(back)))
                    {
                        p++;
                    }
                    if (!world->getContent(World::NW(back)))
                    {
                        p++;
                    }
                }
                else if (back.y < 0) //if on upper side of map, check southeast and southwest
                {
                    if (!world->getContent(World::SE(back)))
                    {
                        p++;
                    }
                    if (!world->getContent(World::SW(back)))
                    {
                        p++;
                    }
                }
            }

            if (p > priority.first)
            {
                priority = std::make_pair(p, optimal[i]);
            }
        }
    }

    return priority;
}
