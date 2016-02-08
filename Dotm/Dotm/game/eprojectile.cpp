/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             31/1/2016
   File name:        eprojectile.cpp

   File description: Implementation of the
   EProjectile class declared in eprojectile.h
   ---------------------------------------------- */

#include "eprojectile.h"
#include "scene.h"
#include "tilemap.h"
#include "command.h"
#include "../util/physics.h"

/* --------------------
   Access Scene Globals
   -------------------- */
extern vec2f g_xLevelBounds;
extern vec2f g_zLevelBounds;

/* --------------
   Public Methods
   -------------- */
EProjectile::EProjectile(const cstring  name,            
                         const Camera*  camera,
                         const Tilemap* levelTilemap,
                         Scene*         scene,
                         const vec3f&   position,                         
                         const vec3f&   targPos,
                         const real32   initRot):

                         Entity(name,
                                {name},
                                camera,
                                levelTilemap,
                                scene,
                                false,
                                position)                                
{
    m_bodies[0]->rotation.y = initRot;
    m_bodies[0]->scale      = {0.5f, 0.5f, 0.5f};    
    m_velocity = targPos - position;
    D3DXVec3Normalize(&m_velocity, &m_velocity);
    m_velocity /= 3.0f;
    m_stamina = 1;
    m_bodies[0]->position.y += 1.0f;
}

EProjectile::~EProjectile()
{

}

void
EProjectile::update()
{
    // Movement
    m_bodies[0]->position += m_velocity;

    if (m_bodies[0]->position.x < g_xLevelBounds.x ||
        m_bodies[0]->position.x > g_xLevelBounds.y ||
        m_bodies[0]->position.z < g_zLevelBounds.x ||
        m_bodies[0]->position.z > g_zLevelBounds.y)
    {
        m_alive = false;
    }

    // Damage Check
    Tile* currTile = m_tileRefs[m_sceneRef->getInternalTilemap()];
    std::vector<Entity*> nearbyEntities = currTile->t_entities;

    for (auto iter = nearbyEntities.begin();
              iter != nearbyEntities.end();
            ++iter)
    {
        if (*iter == this || 
            !(*iter)->isEnemy() ||
            !(*iter)->isAlive()) continue;

        if (physics::intersectionTest(physics::PHYSICS_INTERSECTION_TYPE_SPHERESPHERE,
                                      &m_bodies[0]->getCollidableGeometry(),
                                      &(*iter)->getBody()->getCollidableGeometry()))
        {
            DamageCommand* selfDamage = new DamageCommand(1, this);
            DamageCommand* enemDamage = new DamageCommand(1, *iter);

            selfDamage->execute();
            enemDamage->execute();

            break;
        }
        
    }
    Entity::update();
}

void
EProjectile::damage(const int32 damage)
{
    m_stamina -= damage;
    if (!m_stamina) m_alive = false;
}
