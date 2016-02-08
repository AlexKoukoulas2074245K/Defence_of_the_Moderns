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
#include "../game/eaiminion.h"
#include "../game/camera.h"
#include "../game/scene.h"
#include "../game/tilemap.h"
#include "../game/basemanager.h"
#include "../systemmonitor.h"
#include "../util/logging.h"
#include "../handlers/inputhandler.h"
#include <ctime>
#include <random>
#include <string>

static uint64 frameCounter   = 0;
static uint64 lastUpdateTick = 0;
static uint64 lastRenderTick = 0;
static bool   updateTicks    = false;

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
                       m_levelGrid,
                       m_scene,
                       Entity::ENTITY_PROPERTY_STATIC,
                       {0.0f, -0.6f, 0.0f},
                       "debug_cyan"))

{ 
    m_baseManager = new BaseManager(m_scene, m_levelGrid, m_camera);
    Renderer::get()->setCamera(m_camera);
    m_scene->addLight(m_sun);

    m_sky->loadNewTexture("sky");        
    m_sky->scale.x = 4.0f;
    m_sky->scale.y = 2.0f;        
    m_sky->position.z = 0.1f;    
        
    m_field->getBody()->scale.x = 50.0f;
    m_field->getBody()->scale.z = 50.0f;             
    
        
    m_pointLights[0] = new PointLight({0.2f, 0.1f, 0.1f, 1.0f}, {0.6f, 0.3f, 0.3f, 1.0f}, m_levelGrid->getTilePos3f(2, 3), 10.0f);
    m_pointLights[1] = new PointLight({0.1f, 0.2f, 0.1f, 1.0f}, {0.3f, 0.6f, 0.3f, 1.0f}, m_levelGrid->getTilePos3f(2, 7), 10.0f);
    m_pointLights[2] = new PointLight({0.1f, 0.1f, 0.2f, 1.0f}, {0.3f, 0.3f, 0.6f, 1.0f}, m_levelGrid->getTilePos3f(5, 3), 10.0f);
    m_pointLights[3] = new PointLight({0.2f, 0.2f, 0.2f, 1.0f}, {0.6f, 0.6f, 0.6f, 1.0f}, m_levelGrid->getTilePos3f(5, 7), 10.0f);

    for (size_t i = 0;
                i < 4;
              ++i)
    {
        m_scene->addLight(m_pointLights[i]);
    }

}

PlayState::~PlayState()
{ 
    if (m_camera)      delete m_camera;
    if (m_sysmonitor)  delete m_sysmonitor;
    if (m_sun)         delete m_sun;      
    if (m_sky)         delete m_sky;    
    if (m_scene)       delete m_scene;
    if (m_levelGrid)   delete m_levelGrid;    
    if (m_baseManager) delete m_baseManager;

    delete m_pointLights[0];
    delete m_pointLights[1];
    delete m_pointLights[2];
    delete m_pointLights[3];
}

void
PlayState::update()
{      
    uint64 updateStart = m_sysmonitor->getTimeMS();

    if (InputHandler::get()->isTapped(InputHandler::KEY_E))
    {
        
        std::srand((uint32)std::time(NULL));
        int minionType = std::rand() % 3;
        EAIMinion* newEnemy = nullptr;

        if(minionType == 0)
        {
            newEnemy = new EAIMinion("first_turret",
                                     {"turret01_top", "turret01_base"},
                                     m_camera,
                                     m_levelGrid,
                                     m_scene,
                                     true,
                                     m_levelGrid->getTilePos3f(5, 0),
                                     {0.1f, 0.1f, 0.1f},
                                     "grass");
        }
        else if(minionType == 1)
        {
            newEnemy = new EAIMinion("first_turret",
                                     {"turret02_top", "turret02_base"},
                                     m_camera,
                                     m_levelGrid,
                                     m_scene,
                                     true,
                                     m_levelGrid->getTilePos3f(5, 0),
                                     {0.1f, 0.1f, 0.1f},
                                     "grass");
        }
        else
        {
            newEnemy = new EAIMinion("first_turret",
                                     {"turret03_top", "turret03_base"},
                                     m_camera,
                                     m_levelGrid,
                                     m_scene,
                                     true,
                                     m_levelGrid->getTilePos3f(5, 0),
                                     {0.1f, 0.1f, 0.1f},
                                     "grass");
        }

        newEnemy->getBody()->rotation.y = PI_FL;
        newEnemy->findPathTo(m_levelGrid->getTilePos3f(5, 10), true);
    }
    
    m_camera->update();        
    m_baseManager->update();
    m_scene->update();    
    m_sysmonitor->update(); 

    frameCounter++;
    updateTicks = false;

    if(frameCounter == 20)
    {
        updateTicks = true;
        frameCounter = 0;
    }

    if(updateTicks)
    {
        lastUpdateTick = m_sysmonitor->getTimeMS() - updateStart;
    }
}

void
PlayState::render()
{
    Renderer::get()->beginFrame();
    Renderer::get()->renderMesh(m_sky);

    //m_levelGrid->renderDebug(1, true);
    //m_scene->renderDebug();
    static int frameCounter = 0;
    frameCounter++;
    
    uint64 renderStart = m_sysmonitor->getTimeMS();
    Renderer::get()->renderScene(m_scene);    
    

    if(updateTicks)
    {
        lastRenderTick = m_sysmonitor->getTimeMS() - renderStart;
        updateTicks = false;
    }

    // Profiling
    Renderer::get()->renderString("Fps: ", -0.95f, 0.95f); 
    Renderer::get()->renderString(std::to_string(m_sysmonitor->getFPS()).c_str(), -0.7f, 0.95f);
    Renderer::get()->renderString("Cpu: ", -0.95f, 0.80f);
    Renderer::get()->renderString(std::string(std::to_string(m_sysmonitor->getCpuUsagePerc()) + "%").c_str(), -0.70f, 0.80f);
    Renderer::get()->renderString("Mem: ", -0.95f, 0.65f);
    Renderer::get()->renderString(std::string(std::to_string(m_sysmonitor->getMemUsage()) + "mb").c_str(), -0.70f, 0.65f);            
    Renderer::get()->renderString(("Update: " + std::to_string(lastUpdateTick)).c_str(), -0.95f, 0.5f);
    Renderer::get()->renderString(("Render: " + std::to_string(lastRenderTick)).c_str(), -0.95f, 0.35f);
    Renderer::get()->endFrame();
}