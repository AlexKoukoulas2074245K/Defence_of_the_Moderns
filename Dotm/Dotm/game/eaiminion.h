/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             2/2/2016
   File name:        eaminion.h

   File description: A generic AI controlled
   minion, armed with pathfinding
   -------------------------------------------- */

#pragma once
#include "entity.h"
#include <thread>
#include <mutex>

class Healthbar;
class EAIMinion: public Entity
{

public:

    EAIMinion(const cstring               name,
              const std::vector<cstring>& meshNames,           
              const Camera*               camera,
              const Tilemap*              levelTilemap,
              Scene*                      scene,
              const bool                  optSelectable = true,
              const vec3f&                optPosition = vec3f(),
              const vec3f&                optVelocity = vec3f(),
              const cstring               optExternTexName = nullptr);

    virtual
    ~EAIMinion();
    
    virtual void
    update() override;

    virtual void
    renderInternalComponents() override;

    virtual void
    damage(const int32 damage) override;

    void
    findPathTo(const vec3f&   target,                
               const bool     erasePrevious);    

    void
    recalculatePath();

private:

    Healthbar*          m_healthbar;
    std::list<Command*> m_path;
    std::thread         m_pathThread;
    std::mutex          m_pathMutex;
    vec3f               m_velocity;
    vec3f               m_goalPosition;
    
};
