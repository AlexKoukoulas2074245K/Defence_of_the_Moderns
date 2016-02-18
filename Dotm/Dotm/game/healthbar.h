/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/2/2016
   File name:        healthbar.h

   File description: A general purpose healthbar
   class used to represent all the types of 
   minion healthbars in game
   --------------------------------------------- */

#pragma once

#include "../rendering/renderer.h"
#include "../util/math.h"
#include "../dotmdef.h"
#include "../util/strings.h"
#include <vector>

class Mesh;
class Healthbar
{
public:

    Healthbar(const cstring healthbarType,              
              const vec3f&  minionPos,
              const int32   maxHitpoints);

    virtual
    ~Healthbar();

    Healthbar(const Healthbar& rhs) = delete;

    Healthbar&
    operator = (const Healthbar& rhs) = delete;
    
    virtual void
    update(const vec3f& minionPos);

    void
    render();

    void
    setHitpoints(const int32 hitpoints);

private:

    void
    positionComponents(const vec3f& minionPos);

private:

    static const real32 HEALTHBAR_INIT_SCALE_X;
    static const real32 HEALTHBAR_VER_PADDING;
    static const real32 HEALTHBAR_LERP_SPEED;

private:

    std::vector<Mesh*> m_components;    
    int32              m_maxHitpoints;
    int32              m_curHitpoints;
    real32             m_targetScale;    
    real32             m_decreaseOffset;
};