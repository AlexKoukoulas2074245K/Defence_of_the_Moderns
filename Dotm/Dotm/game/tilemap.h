/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             11/1/2016
   File name:        tilemap.h

   File description: An encapsulation
   of the underlying tile map representation of 
   a level
   -------------------------------------------- */

#pragma once

#include "../dotmdef.h"
#include "../util/math.h"

class Tilemap
{
public:

    struct Tile
    {
        vec2f position;
    };

public:

    static const real32 TILEMAP_TILESIZE;

public:

    Tilemap();

    ~Tilemap();

    Tilemap(const Tilemap& rhs) = delete;

    Tilemap&
    operator = (const Tilemap& rhs) = delete;

    void
    renderDebug();

private:

    uint32 m_tileRows, m_tileCols;
    vec3f  m_position;

};