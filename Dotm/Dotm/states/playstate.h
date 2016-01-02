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
class Camera;
class SystemMonitor;
class PlayState: public AGameState
{
public:
	
    PlayState();

    ~PlayState();

    PlayState(const PlayState& rhs) = delete;
    
    PlayState&
    operator = (const PlayState& rhs) = delete;

    void
    update();

    void
    render();

private:    
    
    Mesh*          m_meshes[6];    
    Camera*        m_camera;
    SystemMonitor* m_sysmonitor;
};