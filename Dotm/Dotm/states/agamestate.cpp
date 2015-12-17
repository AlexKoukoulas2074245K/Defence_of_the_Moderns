/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        agamestate.cpp

   File description: Partial implementation
   of the AGameState abstract class declared
   in agamestate.h
   ------------------------------------------ */

#include "agamestate.h"

/* --------------
   Public Methods
   -------------- */
AGameState::AGameState():

    m_finished(false)
{

}

AGameState::~AGameState(){}

bool
AGameState::isFinished() logical_const
{
    return m_finished;
}