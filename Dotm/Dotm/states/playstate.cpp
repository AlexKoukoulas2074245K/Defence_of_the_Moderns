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
#include "../rendering/lights.h"
#include "../game/entity.h"
#include "../game/camera.h"
#include "../game/scene.h"
#include "../systemmonitor.h"
#include "../handlers/inputhandler.h"
#include "../util/physics.h"
#include "../util/logging.h"


/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():
    
    m_scene(new Scene),
    m_camera(new WorldViewCamera),
    m_sysmonitor(new SystemMonitor),
    m_field(new Mesh("sample_plane", Mesh::MESH_TYPE_NORMAL, m_scene)),
    m_sky(new Mesh("sky", Mesh::MESH_TYPE_HUD, nullptr)),
    m_sun(new DirectionalLight(vec4f(0.4f, 0.4f, 0.4f, 1.0f),
                               vec4f(0.8f, 0.8f, 0.8f, 1.0f),
                               vec3f(0.0f, 0.0f, 1.0f),
                               m_scene))   
{
    m_field->loadNewTexture("grass");    
    m_field->scale.x     = 50.0f;
    m_field->scale.z     = 50.0f;
    m_field->position.y -= 0.7f;

    m_sky->loadNewTexture("sky");        
    m_sky->scale.x = 4.0f;
    m_sky->scale.y = 2.0f;        

    uint64 start = SystemMonitor::getTimeMS();
    
    /*for (size_t i = 0; i < 10; ++i)
    {*/
        m_entities[0] = new Entity("first_turret", {"turret01_base", "turret01_top"},  m_scene, {-5.0f, 0.0f, 0.0f});
        m_entities[1] = new Entity("second_turret", {"turret01_base", "turret01_top"}, m_scene, {0.0f, 0.0f, 0.0f});
        m_entities[2] = new Entity("third_turret", {"turret03_base", "turret03_top"},  m_scene, {5.0f, 0.0f, 0.0f});
/*
        if (i < 9)
        {
            delete m_entities[0];
            delete m_entities[1];
            delete m_entities[2];
        }
    }
    
    */
    logstring("Time elapsed: ");
    logvar(SystemMonitor::getTimeMS() - start);
    logline(" ms");

    
    Renderer::get()->setCamera(m_camera);
}

PlayState::~PlayState()
{ 
    if (m_camera)     delete m_camera;
    if (m_sysmonitor) delete m_sysmonitor;
    if (m_sun)        delete m_sun;    
    if (m_field)      delete m_field;
    if (m_sky)        delete m_sky;
    
    for (size_t i = 0; i < 3; ++i) if(m_entities[i]) delete m_entities[i];

    if (m_scene) delete m_scene;
}

void
PlayState::update()
{
    m_camera->update();
    m_sysmonitor->update();
    m_scene->update();    
}

void
PlayState::render()
{
    Renderer::get()->beginFrame();
    Renderer::get()->renderMesh(m_sky);
    Renderer::get()->renderScene(m_scene);    

    // Profiling
    Renderer::get()->renderString("Fps: ", -0.95f, 0.95f); 
    Renderer::get()->renderString(std::to_string(m_sysmonitor->getFPS()).c_str(), -0.7f, 0.95f);
    Renderer::get()->renderString("Cpu: ", -0.95f, 0.80f);
    Renderer::get()->renderString(std::string(std::to_string(m_sysmonitor->getCpuUsagePerc()) + "%").c_str(), -0.70f, 0.80f);
    Renderer::get()->renderString("Mem: ", -0.95f, 0.65f);
    Renderer::get()->renderString(std::string(std::to_string(m_sysmonitor->getMemUsage()) + "mb").c_str(), -0.70f, 0.65f);    
    /*Renderer::get()->renderString("Touching: ", -0.95f, 0.5f);
    Renderer::get()->renderString(std::to_string(touched).c_str(), -0.3f, 0.5f);*/
    Renderer::get()->endFrame();
}