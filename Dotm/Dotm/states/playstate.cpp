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
#include "../game/eturret.h"
#include "../game/pathfinding.h"
#include "../systemmonitor.h"
#include "../util/logging.h"
#include "../util/physics.h"
#include "../handlers/inputhandler.h"

#define NPOINTLIGHTS 4

/* --------------
   Public Methods
   -------------- */
PlayState::PlayState():
    
    m_scene(new Scene),
    m_camera(new WorldViewCamera),
    m_sysmonitor(new SystemMonitor),
    m_levelGrid(new Tilemap(11, 11, 8.0f, {0.0f, 0.0f, 0.0f})),
    m_sky(new Mesh("sky", Mesh::MESH_TYPE_HUD)),    
    m_sun(new DirectionalLight(vec4f(0.4f, 0.4f, 0.4f, 1.0f),
                               vec4f(0.8f, 0.8f, 0.8f, 1.0f),
                               vec3f(0.0f, 0.0f, 1.0f))),
    m_field(new Entity("field",
                       {"sample_plane"},
                       m_camera,
                       Entity::ENTITY_PROPERTY_STATIC,
                       {0.0f, -0.6f, 0.0f},
                       "debug_cyan"))

{ 
    m_testTurret = new ETurret("turret02",                               
                               m_camera,
                               m_scene,
                               m_levelGrid->getTilePos3f(0, 6),
                               0.01f,
                               40.0f,
                               60);

    m_testTurret2 = new ETurret("turret03",                               
                               m_camera,
                               m_scene,
                               m_levelGrid->getTilePos3f(5, 6),
                               0.003f,
                               40.0f,
                               60);

    
    m_scene->addEntity(m_testTurret);
    m_scene->addEntity(m_testTurret2);
    m_scene->addLight(m_sun);
    
    m_sky->loadNewTexture("sky");        
    m_sky->scale.x = 4.0f;
    m_sky->scale.y = 2.0f;        
    m_sky->position.z = 0.1f;    

    m_field->getBody()->scale.x = 50.0f;
    m_field->getBody()->scale.z = 50.0f;
    m_scene->addEntity(m_field);
        
    m_enemy = new AIEntity("first_turret",
                           {"sample_plane"},
                           m_camera,
                           true,
                           m_levelGrid->getTilePos3f(2, 0),
                           {0.1f, 0.1f, 0.1f},
                           "grass");

    m_scene->addEntity(m_enemy);

    m_testTurret->setTargetEnemy(m_enemy);
    m_testTurret2->setTargetEnemy(m_enemy);

    Renderer::get()->setCamera(m_camera);

#if 0
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
        
#endif

#if 0
    m_pointLights = new Light*[NPOINTLIGHTS];
    m_pointLights[0] = new PointLight({0.2f, 0.2f, 0.2f, 1.0f}, {0.6f, 0.6f, 0.6f, 1.0f}, m_levelGrid->getTilePos3f(0, 0), 10.0f);
    m_pointLights[1] = new PointLight({0.2f, 0.2f, 0.2f, 1.0f}, {0.6f, 0.6f, 0.6f, 1.0f}, m_levelGrid->getTilePos3f(3, 5), 10.0f);
    m_pointLights[2] = new PointLight({0.2f, 0.2f, 0.2f, 1.0f}, {0.6f, 0.6f, 0.6f, 1.0f}, m_levelGrid->getTilePos3f(5, 0), 10.0f);
    m_pointLights[3] = new PointLight({0.2f, 0.2f, 0.2f, 1.0f}, {0.6f, 0.6f, 0.6f, 1.0f}, m_levelGrid->getTilePos3f(4, 4), 10.0f);

    for (size_t i = 0;
              i < NPOINTLIGHTS;
            ++i)
    {
        m_scene->addLight(m_pointLights[i]);
    }
#endif

}

PlayState::~PlayState()
{ 
    if (m_camera)      delete m_camera;
    if (m_sysmonitor)  delete m_sysmonitor;
    if (m_sun)         delete m_sun;    
    if (m_field)       delete m_field;
    if (m_sky)         delete m_sky;
    if (m_scene)       delete m_scene;
    if (m_levelGrid)   delete m_levelGrid;
    if (m_testTurret)  delete m_testTurret;
    if (m_testTurret2) delete m_testTurret2;
    if (m_enemy)       delete m_enemy;
}

void
PlayState::update()
{
    if (InputHandler::get()->isTapped(InputHandler::KEY_Q)) 
    {        
        m_enemy->findPathTo(m_levelGrid->getTilePos3f(2, 6), m_levelGrid, true);        
    }

    if (InputHandler::get()->isTapped(InputHandler::KEY_E))
    {
        delete m_testTurret2;

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
    //m_levelGrid->renderDebug(1, true);
    //m_testTurret->renderDebug();
    //m_testTurret2->renderDebug();
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