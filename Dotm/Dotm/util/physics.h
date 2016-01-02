/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             2/1/2016
   File name:        physics.h

   File description: A collections of helpful 
   physics related functions to be used throughout
   the game
   ------------------------------------------------ */

#pragma once

#include "../rendering/mesh.h"

class Mesh;
class Camera;
namespace physics
{
    bool 
    isPicked(const Mesh* mesh, const Camera* camera);

    bool
    raySphereIntersect(const vec3f& rayOrigin,
                       const vec3f& rayDirection,
                       const real32 sphereRad);
}