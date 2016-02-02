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
#include "../util/physics.h"
#include "../util/stringutils.h"

#ifdef _DEBUG
#include "../rendering/renderer.h"
#endif

/* --------------
   Public Methods
   -------------- */
ETurret::ETurret(const cstring name,                 
                 const Camera* camera,
                 Scene*        scene,
                 const vec3f&  position,
                 const real32  rotVel,
                 const real32  range,
                 const uint32  reloadFrames):

                 Entity(name,
                        { string_utils::strcat(name, "_top").c_str(),
                          string_utils::strcat(name, "_base").c_str() },
                        camera, 
                        scene,
                        ENTITY_PROPERTY_SELECTABLE, 
                        position, 
                        nullptr),
                 
                 m_targetEnemy(nullptr),
                 m_rotVel(rotVel),
                 m_range(range),
                 m_reloadFrames(reloadFrames),
                 m_reloadCounter(reloadFrames)
                     
{
    m_rangeSphere = new math::Sphere(position, m_range);    
}

ETurret::~ETurret()
{
    if (m_rangeSphere) delete m_rangeSphere;
}

void
ETurret::update()
{
    Entity::update();
    
    if (!m_targetEnemy)
    {
        auto globEntities = m_sceneRef->getEntities();

        for (auto citer = globEntities.cbegin();
                  citer != globEntities.cend();
                ++citer)
        {
            if (*citer == this ||
                !(*citer)->isEnemy() ||
                !(*citer)->isAlive() ||
                !isEnemyInSight(*citer))
            {
                continue;
            }

            m_targetEnemy = *citer;
            break;
        }
    }
    else
    {
        if (!m_targetEnemy->isAlive() || !isEnemyInSight(m_targetEnemy)) 
        {
            m_targetEnemy = nullptr;
            return;
        }

        real32 xarg = m_targetEnemy->getBody()->position.x - m_bodies[0]->position.x;
        real32 zarg = m_targetEnemy->getBody()->position.z - m_bodies[0]->position.z;
        INT rotReached = math::lerpf(m_bodies[0]->rotation.y, math::atan2f(xarg, zarg), m_rotVel);

        if (rotReached && !m_reloadCounter--)
        {
            m_reloadCounter = m_reloadFrames;

            EProjectile* projectile = new EProjectile("bullet01",
                                                      m_cameraRef,
                                                      m_sceneRef,
                                                      m_bodies[0]->position,                                                      
                                                      m_targetEnemy->getBody()->position,
                                                      m_bodies[0]->rotation.y);            
        }
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