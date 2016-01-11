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

    Tilemap(const size_t nRows, 
            const size_t nCols,
            const real32 tileSize,
            const vec3f& origin);

    ~Tilemap();

    Tilemap(const Tilemap& rhs) = delete;

    Tilemap&
    operator = (const Tilemap& rhs) = delete;

    vec2f
    getTilePos2f(const size_t col, const size_t row);

    vec3f
    getTilePos3f(const size_t col, const size_t row);

    void
    renderDebug();

private:
    
    size_t m_nRows, m_nCols;
    real32 m_tileSize;
    Tile** m_tiles;
    vec3f  m_origin;
};