/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             11/1/2016
   File name:        tilemap.cpp

   File description: Implementation of the
   Tilemap class declared in tilemap.h
   -------------------------------------------- */

#include "../rendering/renderer.h"
#include "tilemap.h"
#include "camera.h"

/* --------------
   Public Methods
   -------------- */   
Tilemap::Tilemap(const size_t nRows,
                 const size_t nCols,
                 const real32 tileSize,
                 const vec3f& origin):

                 m_nRows(nRows),
                 m_nCols(nCols),
                 m_tileSize(tileSize),
                 m_origin(origin)

{
    m_tiles = new Tile**[nRows];

    for (size_t y = 0;
                y < m_nRows;
              ++y)
    {
        m_tiles[y] = new Tile*[nCols];

        for (size_t x = 0;
                    x < m_nCols;
                  ++x)
        {
            m_tiles[y][x] = new Tile;
            m_tiles[y][x]->t_position.x = m_origin.x -
                ((real32(x) - real32(m_nCols) / 2) * m_tileSize) - m_tileSize / 2;

            m_tiles[y][x]->t_position.y = m_origin.y -
                ((real32(y) - real32(m_nRows) / 2) * m_tileSize) - m_tileSize / 2;
        }
    }
}

Tilemap::~Tilemap()
{
    for (size_t y = 0;
                y < m_nRows;
              ++y)
    {
        for (size_t x = 0;
                    x < m_nCols;
                  ++x)
        {
            delete m_tiles[y][x];
        }

        delete m_tiles[y];
    }
    delete m_tiles;
}

size_t
Tilemap::getCol(const real32 x) logical_const
{
    // In both getCol to calculate the respective tilemap coordinate
    // for the input, we need to add the offset of the tilemap itself
    // (the tilemap's origin is at its center not at the cell 0,0)
    real32 addedXOffset = x - (m_nCols / 2.0f) * m_tileSize;
    return size_t(math::absf(addedXOffset) / m_tileSize);
}

size_t
Tilemap::getRow(const real32 z) logical_const
{
    real32 addedZOffset = z - 2.0f - (m_nRows / 2.0f) * m_tileSize;
    return size_t(math::absf(addedZOffset) / m_tileSize);
}

vec2f
Tilemap::getTilePos2f(const size_t col, const size_t row) logical_const
{
    return m_tiles[row][col]->t_position;
}

vec3f
Tilemap::getTilePos3f(const size_t col, const size_t row) logical_const
{ 
    return math::getVec3f(m_tiles[row][col]->t_position);
}

Tile*
Tilemap::getTile(const size_t col, const size_t row) bitwise_const
{
    return m_tiles[row][col];
}

void
Tilemap::renderDebug()
{
#ifdef _DEBUG
    math::GeoPlane debugPlane({}, {});

    for (size_t y = 0;
                y < m_nRows; 
              ++y)
    {
        for (size_t x = 0;
                    x < m_nCols;
                  ++x)
        {
            debugPlane.setPosition(math::getVec3f(m_tiles[y][x]->t_position));
            debugPlane.setDimensions({m_tileSize, m_tileSize});
            Renderer::get()->renderPrimitive(Renderer::PLANE, &debugPlane, true);                            
        }
    }

#endif
}
