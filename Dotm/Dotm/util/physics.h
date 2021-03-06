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

class  Mesh;
class  Camera;
class  Tilemap;
struct Tile;
namespace physics
{
    const uint32 PHYSICS_INTERSECTION_TYPE_SPHERESPHERE  = 1;
    const uint32 PHYSICS_INTERSECTION_TYPE_RAYSPHERE     = 2;
    const uint32 PHYSICS_INTERSECTION_TYPE_FRUSTUMSPHERE = 3;

    void
    mouseToRay(const Camera* camera, math::Ray& outRay);

    Tile*
    getHighlightedTile(const Camera* camera, const Tilemap* tilemap);

    bool
    isPicked(const Mesh* mesh, const Camera* camera);

    bool
    intersectionTest(const uint32          intersectionType,
                     const math::Geometry* first,
                     const math::Geometry* second);     
}