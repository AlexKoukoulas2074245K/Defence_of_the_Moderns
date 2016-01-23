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
#include "../game/tilemap.h"
#include "../game/pathfinding.h"
#include "../systemmonitor.h"
#include "../util/logging.h"
#include "../handlers/inputhandler.h"
#include <ctime>
#include <random>
#define NENTITIES 3

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():
    
    m_scene(new Scene),
    m_camera(new WorldViewCamera),
    m_sysmonitor(new SystemMonitor),
    m_levelGrid(new Tilemap(7, 7, 8.0f, {0.0f, 0.0f, 0.0f})),
    m_sun(new DirectionalLight(vec4f(0.4f, 0.4f, 0.4f, 1.0f),
                               vec4f(0.8f, 0.8f, 0.8f, 1.0f),
                               vec3f(0.0f, 0.0f, 1.0f),
                               m_scene))
{ 

    m_sky = new Mesh("sky", Mesh::MESH_TYPE_HUD);
    m_sky->loadNewTexture("sky");        
    m_sky->scale.x = 4.0f;
    m_sky->scale.y = 2.0f;        
    m_sky->position.z = 0.1f;
    
    m_field = new Entity("field",
                        {"sample_plane"},                        
                        m_camera, 
                        Entity::ENTITY_PROPERTY_STATIC, 
                        {0.0f, -0.7f, 0.0f}, 
                        "cyan");        

    m_field->getBody()->scale.x = 50.0f;
    m_field->getBody()->scale.z = 50.0f;
    m_scene->addEntity(m_field);

    uint64 start = SystemMonitor::getTimeMS();
    
    
    std::srand((uint32) std::time(NULL));
    m_entities = new Entity*[NENTITIES];
    for (size_t i = 0;
                i < NENTITIES;
              ++i)
    {
        if (i < NENTITIES / 3) m_entities[i] = new Entity("first_turret",
                               {"turret01_top", "turret01_base"},                               
                               m_camera,
                               Entity::ENTITY_PROPERTY_SELECTABLE, 
                               m_levelGrid->getTilePos3f(3, 5));    

        else if (i < NENTITIES * 2/3) m_entities[i] = new Entity("second_turret",
                               {"turret02_top", "turret02_base"},                               
                               m_camera,
                               Entity::ENTITY_PROPERTY_SELECTABLE, 
                               m_levelGrid->getTilePos3f(4, 5));

        else  m_entities[i] = new Entity("third_turret",
                               {"sample_sphere"},                               
                               m_camera,
                               Entity::ENTITY_PROPERTY_SELECTABLE, 
                               m_levelGrid->getTilePos3f(5, 5),
                               "debug");
    }
    
    m_levelGrid->getTile(0, 4)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(1, 4)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(2, 4)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(3, 4)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(4, 4)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(5, 4)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(1, 0)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(1, 1)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(1, 2)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(3, 1)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(3, 2)->t_flags |= TILE_FLAG_SOLID;
    m_levelGrid->getTile(3, 3)->t_flags |= TILE_FLAG_SOLID;

    for (size_t i = 0;
                i < NENTITIES;
              ++i)
    {
        m_scene->addEntity(m_entities[i]);
    }

    m_sysmonitor->loglap(start, SystemMonitor::getTimeMS);

    
    Renderer::get()->setCamera(m_camera);
}

PlayState::~PlayState()
{ 
    if (m_camera)     delete m_camera;
    if (m_sysmonitor) delete m_sysmonitor;
    if (m_sun)        delete m_sun;    
    if (m_field)      delete m_field;
    if (m_sky)        delete m_sky;
    if (m_scene)      delete m_scene;
    if (m_levelGrid)  delete m_levelGrid;
    
    for (size_t i = 0;
                i < NENTITIES;
              ++i)
    {
        if(m_entities[i]) delete m_entities[i];
    }

    delete m_entities;
}

void
PlayState::update()
{
    if (InputHandler::get()->isTapped(InputHandler::KEY_Q)) 
    {        
        m_entities[0]->findPathTo(m_levelGrid->getTilePos3f(0, 0), m_levelGrid, true);
    }
    m_camera->update();    
    m_sysmonitor->update();   
    m_scene->update();   
}

void
PlayState::render()
{
    Renderer::get()->beginFrame();
    Renderer::get()->renderMesh(m_sky);
    m_levelGrid->renderDebug();
    Renderer::get()->renderScene(m_scene);

    // Profiling
    Renderer::get()->renderString("Fps: ", -0.95f, 0.95f); 
    Renderer::get()->renderString(std::to_string(m_sysmonitor->getFPS()).c_str(), -0.7f, 0.95f);
    Renderer::get()->renderString("Cpu: ", -0.95f, 0.80f);
    Renderer::get()->renderString(std::string(std::to_string(m_sysmonitor->getCpuUsagePerc()) + "%").c_str(), -0.70f, 0.80f);
    Renderer::get()->renderString("Mem: ", -0.95f, 0.65f);
    Renderer::get()->renderString(std::string(std::to_string(m_sysmonitor->getMemUsage()) + "mb").c_str(), -0.70f, 0.65f);    
    Renderer::get()->endFrame();
}