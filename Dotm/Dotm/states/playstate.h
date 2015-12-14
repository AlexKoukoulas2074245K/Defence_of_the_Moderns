/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        playstate.h
   
   File description: A class encapsulating 
   the logic during the play state of the
   game. It inherits from the abstract class 
   AGameState declared in agamestate.h.
   ------------------------------------------ */

#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H

#include "agamestate.h"

class PlayState: public AGameState
{
public:
	
    PlayState();

    ~PlayState();

    PlayState(const PlayState& rhs) = delete;
    
    PlayState& 
    operator= (const PlayState& rhs) = delete;

    void
    update();

};

#endif