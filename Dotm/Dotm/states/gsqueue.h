/* -------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        gsqueue.h

   File description: A class in charge of
   creating, updating and deleting game states
   ------------------------------------------- */

#pragma once

#include <queue>
#include "../dotmdef.h"

class AGameState;
class GameStateQueue
{
public:

    GameStateQueue();

    ~GameStateQueue();

    GameStateQueue(const GameStateQueue& rhs) = delete;

    GameStateQueue&
    operator =(const GameStateQueue& rhs) = delete;

    void
    update();

    void
    render();

    bool
    isDone() logical_const;

private:

    void
    discardState();

private:

    std::queue<AGameState*> m_states;
    bool m_done;
};