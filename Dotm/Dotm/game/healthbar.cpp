/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/2/2016
   File name:        healthbar.cpp

   File description: Implementation of the
   Healthbar class declared in healthbar.h
   --------------------------------------------- */

#include "healthbar.h"
#include "../rendering/mesh.h"
#include "../util/stringutils.h"
#include "../util/logging.h"

/* ---------------
   Class Constants
   --------------- */
const real32 Healthbar::HEALTHBAR_INIT_SCALE_X = 4.0f;

/* --------------
   Public Methods
   -------------- */
Healthbar::Healthbar(const cstring healthbarType,
                     const vec3f&  minionPos,
                     const int32  maxHitpoints):

                     m_maxHitpoints(maxHitpoints),
                     m_curHitpoints(maxHitpoints),
                     m_targetScale(HEALTHBAR_INIT_SCALE_X),
                     m_decreaseOffset(0.0f)
{
    m_components.push_back(new Mesh(string_utils::strcat(healthbarType, "_back").c_str(),
                                    Mesh::MESH_TYPE_BILLBOARD | Mesh::MESH_LOAD_SAME_TEXTURE));
    m_components.push_back(new Mesh(string_utils::strcat(healthbarType, "_front").c_str(),
                                    Mesh::MESH_TYPE_BILLBOARD | Mesh::MESH_LOAD_SAME_TEXTURE));

    
    m_components[0]->position = minionPos;
    m_components[1]->position = minionPos;

    m_components[0]->scale.x = HEALTHBAR_INIT_SCALE_X;
    m_components[1]->scale.x = HEALTHBAR_INIT_SCALE_X;

    m_components[0]->scale.y = 0.5f;
    m_components[1]->scale.y = 0.5f;
}

Healthbar::~Healthbar()
{
    for (auto iter = m_components.begin();
              iter != m_components.end();
            ++iter)
    {
        delete *iter;
    }

    m_components.clear();
}

void
Healthbar::update(const vec3f& minionPos)
{
    real32 scale0 = m_components[1]->scale.x;
    math::lerpf(m_components[1]->scale.x, m_targetScale, 0.05f);    
    m_decreaseOffset += (scale0 - m_components[1]->scale.x) / 2;

    m_components[0]->position = minionPos;
    m_components[1]->position = minionPos;   
    positionComponents();
    
}

void
Healthbar::render()
{
    Renderer::get()->renderMesh(m_components[0]);
    Renderer::get()->renderMesh(m_components[1]);
}

void
Healthbar::setHitpoints(const int32 hitpoints)
{
    m_curHitpoints = hitpoints;    
    m_targetScale  = (hitpoints * HEALTHBAR_INIT_SCALE_X) / m_maxHitpoints;    
}

/* ---------------
   Private Methods
   --------------- */
void
Healthbar::positionComponents()
{
    m_components[0]->position.y += 3.0f;
    m_components[1]->position.y += 3.0f;

    m_components[1]->position.x += m_decreaseOffset;

    m_components[0]->position.z += 0.1f;
    
    m_components[0]->rotation.x = PI_FL / 4;
    m_components[1]->rotation.x = PI_FL / 4;
}