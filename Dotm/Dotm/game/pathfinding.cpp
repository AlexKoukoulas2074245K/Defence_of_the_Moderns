/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/1/2016
   File name:        pathfinding.cpp

   File description: Implementation of the
   pathfinding algorithms declared in pathfinding.h
   ------------------------------------------------ */

#include "pathfinding.h"
#include "command.h"
#include "tilemap.h"
#include "../util/math.h"
#include <unordered_set>
#include <unordered_map>
#include <list>
#include "../util/logging.h"

/* ---------
   Constants
   --------- */
#define PF_INFINITY 0x7fffff

/* -------------------
   Internal Signatures
   ------------------- */
static uint32
calculateHeuristic(const Tile* a, 
                   const Tile* b);

static const Tile*
findLowestFScore(std::unordered_set<const Tile*>&         nodeSet,
                 std::unordered_map<const Tile*, uint32>& fScores);

static void
findNeighbours(const Tile*             node,
               const Tilemap*          grid,
               std::list<const Tile*>* outNeighbours);

/* ----------------
   Public Functions
   ---------------- */
void
pathfinding::findPath(const Tilemap*         grid, 
                      const Tile*            start,
                      const Tile*            end, 
                      Entity*                entity,
                      std::list<Command*>*   outCommandChain)
{        
    std::unordered_set<const Tile*> closedSet;
    std::unordered_set<const Tile*> openSet;
    openSet.insert(start);

    std::unordered_map<const Tile*, uint32>      fScores;
    std::unordered_map<const Tile*, uint32>      gScores;
    std::unordered_map<const Tile*, const Tile*> finPath;
    
    for (size_t y = 0;
                y < grid->getRows();
              ++y)
    {
        for (size_t x = 0;
                    x < grid->getCols();
                  ++x)
        {
            gScores[grid->getTile(x, y)] = PF_INFINITY;
        }
    }
    gScores[start] = 0;
    fScores[start] = calculateHeuristic(start, end);

    while (!openSet.empty())
    {
        const Tile* current = findLowestFScore(openSet, fScores);
        
        if (current == end)
        {
            for (const Tile* current = end;     // start from the goal
                 finPath.count(current);        // stop when current is not in the path
                 current = finPath.at(current)) // backtrack in the path
            {
                vec3f actualPos = math::getVec3f(current->t_position);
                outCommandChain->push_front(new MoveCommand(actualPos, entity));
            }

            return;
        }

        openSet.erase(current);
        closedSet.insert(current);

        std::list<const Tile*> neighbours;
        findNeighbours(current, grid, &neighbours);

        for (auto citer = neighbours.cbegin();
                  citer != neighbours.cend();
                ++citer)
        {
            if (closedSet.count(*citer)) continue;

            // +1 because neighbours (non-diagonal) will have a heuristic value of 1
            uint32 tentGScore = gScores[current] + 1; 

            if (!openSet.count(*citer))
            {
                openSet.insert(*citer);
            }

            // Not a desired path
            if (tentGScore >= gScores[*citer]) continue;
            
            finPath[*citer] = current;
            gScores[*citer] = tentGScore;
            fScores[*citer] = tentGScore + calculateHeuristic(*citer, end);
        }
    }
}

/* ------------------
   Internal Functions
   ------------------ */
static uint32
calculateHeuristic(const Tile* a, 
                   const Tile* b)
{
    return (math::max2ui(a->t_col, b->t_col) - math::min2ui(a->t_col, b->t_col)) +
           (math::max2ui(a->t_row, b->t_row) - math::min2ui(a->t_row, b->t_row));
}

static const Tile*
findLowestFScore(std::unordered_set<const Tile*>&         nodeSet,
                 std::unordered_map<const Tile*, uint32>& fScores)
{
    uint32 minF         = PF_INFINITY;
    const Tile* minNode = nullptr;

    for (auto citer = nodeSet.cbegin();
              citer != nodeSet.cend();
            ++citer)
    {
        if (fScores[*citer] < minF)
        {
            minF    = fScores[*citer];
            minNode = *citer;
        }
    }

    return minNode;
}

static void
findNeighbours(const Tile*             node,
               const Tilemap*          grid,
               std::list<const Tile*>* outNeighbours)
{
    outNeighbours->clear();

    const Tile* nNeighbour = grid->getTile(node->t_col - 1, node->t_row    );
    const Tile* sNeighbour = grid->getTile(node->t_col + 1, node->t_row    );
    const Tile* wNeighbour = grid->getTile(node->t_col    , node->t_row - 1);
    const Tile* eNeighbour = grid->getTile(node->t_col    , node->t_row + 1);

    if (nNeighbour && !nNeighbour->isSolid()) outNeighbours->push_back(nNeighbour);
    if (sNeighbour && !sNeighbour->isSolid()) outNeighbours->push_back(sNeighbour);
    if (wNeighbour && !wNeighbour->isSolid()) outNeighbours->push_back(wNeighbour);
    if (eNeighbour && !eNeighbour->isSolid()) outNeighbours->push_back(eNeighbour);
}