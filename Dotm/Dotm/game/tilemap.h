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

const uint32 TILE_FLAG_SOLID = 0x01;
const uint32 TILE_FLAG_INVIS = 0x02;

struct Tile
{
    uint32               t_flags;
    vec2f                t_position;
    std::vector<Entity*> t_entities;
    size_t               t_col;
    size_t               t_row;

    inline bool isSolid()     logical_const { return (t_flags & TILE_FLAG_SOLID) != 0; }
    inline bool isInvisible() logical_const { return (t_flags & TILE_FLAG_INVIS) != 0; }
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

    const vec3f&
    getOrigin() logical_const;

    size_t
    getCol(const real32 x) logical_const;

    size_t
    getRow(const real32 z) logical_const;

    size_t
    getCols() logical_const;

    size_t
    getRows() logical_const;

    vec2f
    getTilePos2f(const size_t col, const size_t row) logical_const;

    vec3f
    getTilePos3f(const size_t col, const size_t row) logical_const;

    Tile*
    getTile(const size_t col, const size_t row) bitwise_const;

    Tile*
    getTile(const vec3f& position) bitwise_const;
    
    void
    renderDebug(const uint32 color,
                const bool wireframe);

protected:
    
    size_t  m_nRows, m_nCols;
    real32  m_tileSize;
    Tile*** m_tiles; // 2D grid of Tile ptrs
    vec3f   m_origin;
    vec2f   m_horBounds;
    vec2f   m_verBounds;

};