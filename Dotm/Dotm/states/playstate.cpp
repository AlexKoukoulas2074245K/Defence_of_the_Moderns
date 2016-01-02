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
#include "../systemmonitor.h"
#include "../handlers/inputhandler.h"
#include "../util/physics.h"
#include "../util/logging.h"


/* -------------
   Internal Vars
   ------------- */   
static int32 touched(-1);

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():
    
    m_camera(new WorldViewCamera),
    m_sysmonitor(new SystemMonitor)
{         
    m_meshes[0] = new Mesh("turret01_base", Mesh::MESH_LOAD_SAME_TEXTURE | Mesh::MESH_TYPE_NORMAL);
    m_meshes[1] = new Mesh("turret01_top",  Mesh::MESH_LOAD_SAME_TEXTURE | Mesh::MESH_TYPE_NORMAL);
    m_meshes[2] = new Mesh("turret02_base", Mesh::MESH_LOAD_SAME_TEXTURE | Mesh::MESH_TYPE_NORMAL);
    m_meshes[3] = new Mesh("turret02_top",  Mesh::MESH_LOAD_SAME_TEXTURE | Mesh::MESH_TYPE_NORMAL);
    m_meshes[4] = new Mesh("turret03_base", Mesh::MESH_LOAD_SAME_TEXTURE | Mesh::MESH_TYPE_NORMAL);
    m_meshes[5] = new Mesh("turret03_top",  Mesh::MESH_LOAD_SAME_TEXTURE | Mesh::MESH_TYPE_NORMAL);
    
    m_meshes[0]->x = -5.0f;
    m_meshes[1]->x = -5.0f;
    
    m_meshes[2]->x =  0.0f;
    m_meshes[3]->x =  0.0f;
    
    m_meshes[4]->x =  5.0f;
    m_meshes[5]->x =  5.0f;
    
    Renderer::get().setCamera(m_camera);
}

PlayState::~PlayState()
{ 
    if (m_camera)                  delete m_camera;
    if (m_sysmonitor)              delete m_sysmonitor;
    for (size_t i = 0; i < 6; ++i) delete m_meshes[i];
}

void
PlayState::update()
{
    m_camera->update();
    m_sysmonitor->update();
   
    touched = -1;
    for (size_t i = 0; i < 6; ++i)
    {
        if (i % 2 && physics::isPicked(m_meshes[i], m_camera)) touched = i;
    }
}

void
PlayState::render()
{
    Renderer::get().beginFrame();
    for (size_t i = 0; i < 6; ++i)
    {
        Renderer::get().renderMesh(m_meshes[i]);        
        if (i % 2) Renderer::get().renderPrimitive(Renderer::RENDERER_PRIMITIVE_SPHERE,
                                                   m_meshes[i]->getPosition(),
                                                   m_meshes[i]->getDimensions(),
                                                   true);
    }

    

    // Profiling
    Renderer::get().renderString("Fps: ", -0.95f, 0.95f); 
    Renderer::get().renderString(std::to_string(m_sysmonitor->getFPS()).c_str(), -0.7f, 0.95f);
    Renderer::get().renderString("Cpu: ", -0.95f, 0.80f);
    Renderer::get().renderString(std::string(std::to_string(m_sysmonitor->getCpuUsagePerc()) + "%").c_str(), -0.70f, 0.80f);
    Renderer::get().renderString("Mem: ", -0.95f, 0.65f);
    Renderer::get().renderString(std::string(std::to_string(m_sysmonitor->getMemUsage()) + "mb").c_str(), -0.70f, 0.65f);
    Renderer::get().renderString("Touching: ", -0.95f, 0.5f);
    Renderer::get().renderString(std::to_string(touched).c_str(), -0.3f, 0.5f);
    Renderer::get().endFrame();
}