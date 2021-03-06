/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             31/1/2016
   File name:        eprojectile.h

   File description: A base class for the 
   various projectiles emitted from defensive 
   turrets in-game
   ---------------------------------------------- */

#pragma once

#include "entity.h"

class EProjectile: public Entity
{    
public:
       
    EProjectile(const cstring  name,            
                const Camera*  camera,
                const Tilemap* levelTilemap,
                Scene*         scene,
                const vec3f&   position,                
                const vec3f&   targPos,
                const real32   initRot);

    ~EProjectile();

    void
    update() override;

    void
    damage(const int32 damage) override;

private:

    vec3f  m_velocity;
    
};