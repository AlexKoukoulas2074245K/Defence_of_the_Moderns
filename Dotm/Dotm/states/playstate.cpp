/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        playstate.cpp
   
   File description: An implementation of the
   derived class PlayState declared in playstate.h
   ----------------------------------------------- */

#include "playstate.h"
#include "../rendering/mesh.h"
#include "../rendering/renderer.h"
#include <cmath>

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():
meshID(internString("material")),
mesh(new Mesh(retrieveString(meshID), false))
{
    
}

PlayState::~PlayState()
{
    if(mesh) delete mesh;
}

void
PlayState::update()
{    
    mesh->scaleX = 0.03f;
    mesh->scaleY = 0.03f;
    mesh->scaleZ = 0.03f;
    mesh->rotY += 0.01f;    
    mesh->y = -4.0f;
}

void
PlayState::render()
{
    Renderer::get().beginFrame();
    Renderer::get().renderMesh(meshID);       
    Renderer::get().endFrame();
}