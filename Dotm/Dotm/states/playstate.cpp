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
#include "../game/camera.h"
#include <cmath>

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():

    m_mesh(new Mesh("material", false)),
    m_camera(new Camera())
{    
    Renderer::get().setCamera(m_camera);
}

PlayState::~PlayState()
{
    if (m_camera) delete m_camera;
    if (m_mesh)   delete m_mesh;
}

void
PlayState::update()
{
    m_camera->update();
    m_mesh->scaleX = 0.03f;
    m_mesh->scaleY = 0.03f;
    m_mesh->scaleZ = 0.03f;
    //mesh->rotY += 0.01f;
    m_mesh->y = -4.0f;    
}

void
PlayState::render()
{
    Renderer::get().beginFrame();
    Renderer::get().renderMesh(m_mesh);       
    Renderer::get().endFrame();
}