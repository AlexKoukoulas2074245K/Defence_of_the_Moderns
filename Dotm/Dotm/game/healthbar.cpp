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
const real32 Healthbar::HEALTHBAR_LERP_SPEED   = 0.05f;
const real32 Healthbar::HEALTHBAR_VER_PADDING  = 3.0f;

/* --------------
   Public Methods
   -------------- */
Healthbar::Healthbar(const cstring healthbarType,                     
                     const vec3f&  minionPos,
                     const int32   maxHitpoints):
                     
                     m_maxHitpoints(maxHitpoints),
                     m_curHitpoints(maxHitpoints),
                     m_targetScale(HEALTHBAR_INIT_SCALE_X),
                     m_decreaseOffset(0.0f)
{
    m_components.push_back(new Mesh(string_utils::strcat(healthbarType, "_back").c_str(),
                                    Mesh::MESH_TYPE_BILLBOARD | Mesh::MESH_LOAD_SAME_TEXTURE));
    m_components.push_back(new Mesh(string_utils::strcat(healthbarType, "_front").c_str(),
                                    Mesh::MESH_TYPE_BILLBOARD | Mesh::MESH_LOAD_SAME_TEXTURE));

    positionComponents(minionPos);
    
    m_components[0]->scale.x = 0.0f;
    m_components[1]->scale.x = HEALTHBAR_INIT_SCALE_X;    

    m_components[0]->scale.y /= 2.0f;
    m_components[1]->scale.y /= 2.0f;
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
    // Essentially to produce the effect of the current stamina bar
    // in front and the lost stamina bar behind (black), we start
    // both objects next to each other with the stamina bar being 
    // at full scale and the lost stamina bar at 0 scale. Whenever
    // stamina is lost, we decrease the scale of the curr stamina bar
    // and increase the scale of the lost stamina bar.

    // Save previous scale
    real32 scale0 = m_components[1]->scale.x;

    // Calculate next scale
    math::lerpf(m_components[1]->scale.x, m_targetScale, HEALTHBAR_LERP_SPEED);    
    m_components[0]->scale.x = HEALTHBAR_INIT_SCALE_X - m_components[1]->scale.x;

    // Calculate the offset needed to position the bars correctly
    m_decreaseOffset += (scale0 - m_components[1]->scale.x) / 2;
        
    positionComponents(minionPos);    
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
Healthbar::positionComponents(const vec3f& minionPos)
{
    // Start by setting the components' position to the minion's position
    m_components[0]->position = minionPos;
    m_components[1]->position = minionPos;

    // Move components above model
    m_components[0]->position.y += HEALTHBAR_VER_PADDING;
    m_components[1]->position.y += HEALTHBAR_VER_PADDING;

    // Position bars next to each other
    m_components[0]->position.x += m_decreaseOffset - HEALTHBAR_INIT_SCALE_X / 2.0f;
    m_components[1]->position.x += m_decreaseOffset;
    
    // Correct rotation to face camera naturally
    m_components[0]->rotation.x = PI_FL / 4;
    m_components[1]->rotation.x = PI_FL / 4;
}