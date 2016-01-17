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
#include <vector>

class Entity;
struct Tile
{
    vec2f t_position;
    std::vector<Entity*> t_entities;
};

class Tilemap
{
public:

    Tilemap(const size_t nRows, 
            const size_t nCols,
            const real32 tileSize,
            const vec3f& origin);

    virtual
    ~Tilemap();

    Tilemap(const Tilemap& rhs) = delete;

    Tilemap&
    operator = (const Tilemap& rhs) = delete;

    size_t
    getCol(const real32 x) logical_const;

    size_t
    getRow(const real32 z) logical_const;

    vec2f
    getTilePos2f(const size_t col, const size_t row) logical_const;

    vec3f
    getTilePos3f(const size_t col, const size_t row) logical_const;

    Tile*
    getTile(const size_t col, const size_t row) bitwise_const;

    void
    renderDebug();

protected:
    
    size_t  m_nRows, m_nCols;
    real32  m_tileSize;
    Tile*** m_tiles; // 2D grid of Tile ptrs
    vec3f   m_origin;
};