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
#include <algorithm>
#include <ctime>

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():
    
    m_meshes(new Mesh*[20]),    
    m_camera(new WorldViewCamera())
{     
    std::srand((uint32)std::time(0));
    
    for (size_t i = 0; i < 20; ++i)
    {
        m_meshes[i] = new Mesh("turret01_top", false);
        m_meshes[i]->x = (std::rand() % 2) == 0 ? (real32) (std::rand() % 80) : (real32)(-std::rand() % 80);
        m_meshes[i]->z = (std::rand() % 2) == 0 ? (real32) (std::rand() % 60) : (real32)(-std::rand() % 60);
        uint32 randnum = std::rand() % 8;
        m_meshes[i]->scaleX = m_meshes[i]->scaleY = m_meshes[i]->scaleZ = randnum > 1 ? (real32) randnum : 1.0f;
    }
    Renderer::get().setCamera(m_camera);
}

PlayState::~PlayState()
{
    if (m_camera) delete m_camera;
    if (m_meshes) for (size_t i = 0; i < 20; ++i) delete m_meshes[i];
    if (m_meshes) delete[] m_meshes;
}

void
PlayState::update()
{
    m_camera->update();    
}

void
PlayState::render()
{
    Renderer::get().beginFrame();
    for (size_t i = 0; i < 20; ++i) Renderer::get().renderMesh(m_meshes[i]);
    Renderer::get().endFrame();
}