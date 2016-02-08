/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        playstate.h
   
   File description: A class encapsulating 
   the logic during the play state of the
   game. It inherits from the abstract class 
   AGameState declared in agamestate.h.
   ------------------------------------------ */

#pragma once

#include "agamestate.h"
#include "../util/strings.h"

class Mesh;
class Entity;
class EAIMinion;
class BaseManager;
class PointLight;
class Camera;
class SystemMonitor;
class Light;
class Scene;
class Tilemap;
class PlayState: public AGameState
{
public:
	
    PlayState();

    ~PlayState();

    PlayState(const PlayState& rhs) = delete;
    
    PlayState&
    operator = (const PlayState& rhs) = delete;

    void
    update() override;

    void
    render() override;

private:

    void
    meshLoading(const size_t i, const cstring meshName);

private:    
    
    Scene*          m_scene;
    EAIMinion*      m_enemy;    
    BaseManager*    m_baseManager;
    Entity*         m_field;        
    Mesh*           m_sky;
    PointLight*     m_pointLights[4];
    Camera*         m_camera;
    SystemMonitor*  m_sysmonitor;
    Light*          m_sun;        
    Tilemap*        m_levelGrid;

};