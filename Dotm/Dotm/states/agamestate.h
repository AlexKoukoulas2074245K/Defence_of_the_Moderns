/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        agamestate.h

   File description: An abstract class
   representing the possible game states
   ------------------------------------------ */

#pragma once

#include "../dotmdef.h"

class AGameState
{
public:

    AGameState();

    virtual ~AGameState() = 0;

    AGameState(const AGameState& rhs) = delete;

    AGameState&
    operator = (const AGameState& rhs) = delete;

    virtual void
    update() = 0;

    virtual void
    render() = 0;

    bool
    isFinished() logical_const;

protected:

    bool m_finished;

};
