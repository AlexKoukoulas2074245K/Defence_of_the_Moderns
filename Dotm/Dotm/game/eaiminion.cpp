/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             2/2/2016
   File name:        eaminion.cpp

   File description: The implementation of the
   EAIMinion class declared in eaiminion.h
   -------------------------------------------- */

#include "eaiminion.h"
#include "pathfinding.h"
#include "command.h"
#include "tilemap.h"
#include "healthbar.h"

/* --------------
   Public Methods
   -------------- */

EAIMinion::EAIMinion(const cstring               name,
                     const std::vector<cstring>& meshNames,           
                     const Camera*               camera,
                     const Tilemap*              levelTilemap,
                     Scene*                      scene,
                     const bool                  optSelectable,   /* true    */
                     const vec3f&                optPosition,     /* vec3f() */
                     const vec3f&                optVelocity,     /* vec3f() */
                     const cstring               optExternTexName /* nullptr */):

                     Entity(name,
                            meshNames,
                            camera,
                            levelTilemap,
                            scene,
                            optSelectable ? ENTITY_PROPERTY_SELECTABLE : 0U,
                            optPosition, 
                            optExternTexName),

                     m_velocity(optVelocity)                   
{
    m_stamina   = 4;
    m_healthbar = new Healthbar("minion_healthbar",                                 
                                m_bodies[0]->position, 
                                m_stamina);    

    m_enemy = true;
}

EAIMinion::~EAIMinion()
{
    for (auto iter = m_path.begin();
        iter != m_path.end();
        ++iter)
    {
        delete *iter;
    }

    if (m_healthbar) delete m_healthbar;

    Tile* currTile = m_levelTMref->getTile(m_bodies[0]->position);      
}

void
EAIMinion::update()
{
    //m_bodies[0]->rotation.y += 0.01f;

    // Movement
    if (m_hasTarget)
    {
        uint32 goalAccum = 0U;
        uint32 nBodies = m_bodies.size();

        for (auto iter = m_bodies.begin();
                  iter != m_bodies.end();
                ++iter)
        {
            goalAccum += math::lerpf((*iter)->position.x, m_targetPos.x, m_velocity.x);
            goalAccum += math::lerpf((*iter)->position.y, m_targetPos.y, m_velocity.y);
            goalAccum += math::lerpf((*iter)->position.z, m_targetPos.z, m_velocity.z);
        }

        // Lerp on goal returns 1, so the entity has reached 
        // the target when all of its bodies' positional coordinates
        // have reached the goal and have returned 1 on lerp. And hence nBodies * 3 (x, y, z)
        if (goalAccum == nBodies * 3)
        {
            // Atomic path update
            m_pathMutex.lock();
            if (m_path.size() > 0)
            {
                m_path.front()->execute();
                m_path.pop_front();
            }
            else
            {
                m_goalPosition = m_bodies[0]->position;
                m_hasTarget = false;
            }
            m_pathMutex.unlock();
        }
    }

    m_healthbar->update(m_bodies[0]->position);
    Entity::update();
}

void
EAIMinion::renderInternalComponents()
{
    m_healthbar->render();
}

void
EAIMinion::damage(const int32 damage)
{
    m_stamina -= damage;
    m_healthbar->setHitpoints(m_stamina);

    if(!m_stamina)
    {
        m_alive = false;
    }
}

void
EAIMinion::findPathTo(const vec3f&   target,                      
                      const bool     erasePrevious)
{      
    m_pathThread = std::thread([=]()
    {
        std::list<Command*> newInstructions;
        pathfinding::findPath(m_levelTMref,
                              m_levelTMref->getTile(m_bodies[0]->position),
                              m_levelTMref->getTile(target), 
                              this,
                              &newInstructions);

        // Atomic path update
        m_pathMutex.lock();
        
        // Replace previous commands in the path with new ones
        if (erasePrevious)
        {
            for (auto iter = m_path.begin();
                      iter != m_path.end();
                    ++iter)
            {
                delete *iter;
            }
            
            m_path.clear();

            for (auto citer = newInstructions.cbegin();
                      citer != newInstructions.cend();
                    ++citer)
            {
                m_path.push_back(*citer);
            }            
        }
        // Keep previous content and add new commands at the end
        else
        {
            for (auto citer = newInstructions.cbegin();
                      citer != newInstructions.cend();
                    ++citer)
            {
                m_path.push_back(*citer);
            }            
        }

        if (m_path.size() > 0)
        {
            m_hasTarget = true;
            m_goalPosition = target;
            m_path.front()->execute();
            m_path.pop_front();
        }
        m_pathMutex.unlock();        
    });
    m_pathThread.detach();
}

void
EAIMinion::recalculatePath()
{    
    findPathTo(m_goalPosition, true);
}