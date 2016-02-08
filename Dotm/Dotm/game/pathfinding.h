/* ---------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/1/2016
   File name:        pathfinding.h

   File description: A collection of pathfinding
   algorithms to be used primarily by the AI
   --------------------------------------------- */

#pragma once

#include <list>

struct Tile;
class  Tilemap;
class  Command;
class  Entity;
namespace pathfinding
{
    bool
    findPath(const Tilemap*         grid, 
             const Tile*            start,
             const Tile*            end, 
             Entity*                entity,
             std::list<Command*>*   outCommandChain);
}