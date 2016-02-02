/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             30/1/2016
   File name:        eturret.h

   File description: A base class for the 
   defensive turrets controlled by the player
   ---------------------------------------------- */

#pragma once

#include "entity.h"

class EProjectile;
class ETurret: public Entity
{
public:

    ETurret(const cstring  name,            
            const Camera*  camera,
            const Tilemap* levelTilemap,
            Scene*         scene,
            const vec3f&   position,
            const real32   rotvel,
            const real32   range,
            const uint32   reloadFrames);

    ~ETurret();

    void
    update() override;

    const Entity*
    getTargetEnemy() logical_const;

    void
    setTargetEnemy(const Entity* enemy);

    void
    renderDebug();

private:

    bool
    isEnemyInSight(const Entity* enemy) logical_const;

private:

    enum TurretState
    {
        SEEKING, ATTACKING
    };

private:

    static const real32 ETURRET_INIT_ROT;

private:

    const Entity*  m_targetEnemy;     
    math::Sphere*  m_rangeSphere;
    TurretState    m_state;
    real32         m_range;
    real32         m_rotVel;
    uint32         m_reloadFrames;
    uint32         m_reloadCounter;
};