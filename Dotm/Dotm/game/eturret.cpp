/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             30/1/2016
   File name:        eturret.cpp

   File description: Implementation of the 
   ETurret class declared in eturret.h
   ---------------------------------------------- */


#include "eturret.h"
#include "eprojectile.h"
#include "scene.h"
#include "tilemap.h"
#include "../util/physics.h"
#include "../util/stringutils.h"
#include "../util/logging.h"

#ifdef _DEBUG
#include "../rendering/renderer.h"
#endif

/* ---------------
   Class Constants
   --------------- */
const real32 ETurret::ETURRET_INIT_ROT = 0.0f;

/* --------------
   Public Methods
   -------------- */
ETurret::ETurret(const cstring  name,                 
                 const Camera*  camera,
                 const Tilemap* levelTilemap,
                 Scene*         scene,
                 const vec3f&   position,
                 const real32   rotVel,
                 const real32   range,
                 const uint32   reloadFrames):

                 Entity(name,
                        { string_utils::strcat(name, "_top").c_str(),
                          string_utils::strcat(name, "_base").c_str() },
                        camera, 
                        levelTilemap,
                        scene,
                        ENTITY_PROPERTY_SELECTABLE, 
                        position, 
                        nullptr),
                              
                 m_state(TurretState::SEEKING),
                 m_targetEnemy(nullptr),
                 m_rotVel(rotVel),
                 m_range(range),
                 m_reloadFrames(reloadFrames),
                 m_reloadCounter(reloadFrames)
                     
{
    m_levelTMref->getTile(position)->t_flags |= TILE_FLAG_SOLID;
    m_turret = true;
    m_rangeSphere = new math::Sphere(position, m_range);    
}

ETurret::~ETurret()
{
    if (m_rangeSphere) delete m_rangeSphere;
    m_levelTMref->getTile(m_bodies[0]->position)->t_flags ^= TILE_FLAG_SOLID;
}

void
ETurret::update()
{
    Entity::update();
    
    switch (m_state)
    {
        case SEEKING:
        {
            auto enemies = m_sceneRef->getEnemies();

            // Try to find enemy in range
            for (auto citer = enemies.cbegin();
                      citer != enemies.cend();
                    ++citer)
            {
                if ((*citer)->isAlive() && isEnemyInSight(*citer))
                {                
                    m_targetEnemy = *citer;
                    m_state       = ATTACKING;
                    break;
                }
            }

            // If no enemy was found interpolate to the intial rotation
            if (!m_targetEnemy)
            {                
                if(math::lerprotf(m_bodies[0]->rotation.y, ETURRET_INIT_ROT, m_rotVel)) 
                {
                    m_bodies[0]->rotation.y = 0.0f;
                }
            }
        }break;

        case ATTACKING:
        {
            if (!m_targetEnemy->isAlive() || !isEnemyInSight(m_targetEnemy))
            {
                m_targetEnemy = nullptr;
                m_state       = SEEKING;
                return;
            }

            real32 xarg = m_targetEnemy->getBody()->position.x - m_bodies[0]->position.x;
            real32 zarg = m_targetEnemy->getBody()->position.z - m_bodies[0]->position.z;            
            real32 goal = math::atan2f(xarg, zarg);            
            
            // Handle negative atan2
            if (goal < 0) goal = 2 * PI_FL + goal;

            INT rotReached = math::lerprotf(m_bodies[0]->rotation.y, goal, m_rotVel);

            if (rotReached && !m_reloadCounter--)
            {
                m_reloadCounter = m_reloadFrames;

                EProjectile* projectile = new EProjectile("bullet01",
                                                           m_cameraRef,
                                                           m_levelTMref,
                                                           m_sceneRef,
                                                           m_bodies[0]->position,
                                                           m_targetEnemy->getBody()->position,
                                                           m_bodies[0]->rotation.y);
            }
        }break;
    }   
}

const Entity*
ETurret::getTargetEnemy() logical_const
{
    return m_targetEnemy;
}

void
ETurret::setTargetEnemy(const Entity* enemy)
{
    m_targetEnemy = enemy;
}

void
ETurret::renderDebug()
{
#ifdef _DEBUG
    Renderer::get()->renderPrimitive(Renderer::SPHERE,
                                     m_rangeSphere, 
                                     Renderer::DebugColor::YELLOW,
                                     true);
#endif
}

/* ---------------
   Private Methods
   --------------- */
bool
ETurret::isEnemyInSight(const Entity* enemy) logical_const
{
    return D3DXVec3Length(&(enemy->getBody()->position - m_bodies[0]->position)) < m_range;
}