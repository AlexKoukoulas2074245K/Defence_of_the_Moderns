/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        playstate.cpp
   
   File description: An implementation of the
   derived class PlayState declared in playstate.h
   ----------------------------------------------- */

#include "playstate.h"
#include "../rendering/mesh.h"

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState()
{
    Mesh mesh("abc", false);
}

PlayState::~PlayState()
{
	
}

void
PlayState::update()
{
	
}