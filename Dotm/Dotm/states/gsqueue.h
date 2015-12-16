/* -------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        gsqueue.h

   File description: A class in charge of
   creating, updating and deleting game states
   ------------------------------------------- */

#pragma once

#include <queue>
#include <memory>

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

    bool
    isDone() const;


private:

    void
    discardState();

private:

    std::queue<std::unique_ptr<AGameState>> m_states;
    bool m_done;
};