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
                        ENTITY_PROPERTY_SELECTABLE, 
                        position, 
                        nullptr),

                 m_sceneRef(scene),
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

    for (auto iter = m_projectiles.begin();
              iter != m_projectiles.end();
            ++iter)
    {
        delete *iter;
    }
}

void
ETurret::update()
{
    Entity::update();
    
    if (m_targetEnemy && isEnemyInSight())
    {
        real32 xarg = m_targetEnemy->getBody()->position.x - m_bodies[0]->position.x;
        real32 zarg = m_targetEnemy->getBody()->position.z - m_bodies[0]->position.z;
        INT goalReached = math::lerpf(m_bodies[0]->rotation.y, math::atan2f(xarg, zarg), m_rotVel);

        if (goalReached && !m_reloadCounter--)
        {
            m_reloadCounter = m_reloadFrames;

            EProjectile* projectile = new EProjectile("bullet01",
                                                      m_cameraRef,
                                                      m_bodies[0]->position,                                                      
                                                      m_targetEnemy->getBody()->position,
                                                      m_bodies[0]->rotation.y);
            m_projectiles.push_back(projectile);
            m_sceneRef->queueAddEntity(projectile);
        }
    }

    for (auto iter = m_projectiles.begin();
              iter != m_projectiles.end();
            ++iter)
    {
        if (!(*iter)->isAlive())
        {
            delete *iter;
            iter = m_projectiles.erase(iter);
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
ETurret::isEnemyInSight() logical_const
{
    return physics::intersectionTest(physics::PHYSICS_INTERSECTION_TYPE_SPHERESPHERE,
                                     m_rangeSphere, 
                                     &m_targetEnemy->getBody()->getCollidableGeometry());
}