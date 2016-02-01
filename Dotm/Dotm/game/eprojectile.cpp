/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             31/1/2016
   File name:        eprojectile.cpp

   File description: Implementation of the
   EProjectile class declared in eprojectile.h
   ---------------------------------------------- */

#include "eprojectile.h"

/* --------------------
   Access Scene Globals
   -------------------- */
extern vec2f g_xLevelBounds;
extern vec2f g_zLevelBounds;

/* --------------
   Public Methods
   -------------- */
EProjectile::EProjectile(const cstring name,            
                         const Camera* camera,
                         const vec3f&  position,                         
                         const vec3f&  targPos,
                         const real32  initRot):

                         Entity(name,
                                {name},
                                camera,
                                false,
                                position)                                
{
    m_bodies[0]->rotation.y = initRot;
    m_bodies[0]->scale      = {0.5f, 0.5f, 0.5f};    
    m_velocity = targPos - position;
    D3DXVec3Normalize(&m_velocity, &m_velocity);
    m_velocity /= 10.0f;
}

EProjectile::~EProjectile()
{

}

void
EProjectile::update()
{
    Entity::update();
    m_bodies[0]->position += m_velocity;

    if (m_bodies[0]->position.x < g_xLevelBounds.x ||
        m_bodies[0]->position.x > g_xLevelBounds.y ||
        m_bodies[0]->position.z < g_zLevelBounds.x ||
        m_bodies[0]->position.z > g_zLevelBounds.y)
    {
        m_alive = false;
    }

}