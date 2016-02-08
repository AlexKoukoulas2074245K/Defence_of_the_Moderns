/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             11/1/2016
   File name:        tilemap.cpp

   File description: Implementation of the
   Tilemap class declared in tilemap.h
   -------------------------------------------- */

#ifdef _DEBUG
#include "../rendering/renderer.h"
#endif
#include "tilemap.h"
#include "camera.h"
#include "../util/logging.h"


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

            m_tiles[y][x]->t_flags = 0U;
            m_tiles[y][x]->t_col   = x;
            m_tiles[y][x]->t_row   = y;

            m_tiles[y][x]->t_position.x = m_origin.x -
                ((real32(x) - real32(m_nCols) / 2) * m_tileSize) - m_tileSize / 2;

            m_tiles[y][x]->t_position.y = m_origin.y -
                ((real32(y) - real32(m_nRows) / 2) * m_tileSize) - m_tileSize / 2;
        }
    }

    m_horBounds[0] = m_tiles[0][0]->t_position.x + m_tileSize / 2.0f;
    m_verBounds[0] = m_tiles[0][0]->t_position.y + m_tileSize / 2.0f;

    m_horBounds[1] = m_tiles[m_nRows - 1][m_nCols - 1]->t_position.x - m_tileSize / 2.0f;
    m_verBounds[1] = m_tiles[m_nRows - 1][m_nCols - 1]->t_position.y - m_tileSize / 2.0f;
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

const vec3f&
Tilemap::getOrigin() logical_const
{
    return m_origin;
}

size_t
Tilemap::getCol(const real32 x) logical_const
{
    // In both getCol and getRow, in order to calculate the respective tilemap 
    // coordinate for the input, we need to add the offset of the tilemap itself
    // (the tilemap's origin is at its center not at the cell 0,0)
    real32 addedXOffset = x - (m_nCols / 2.0f) * m_tileSize;
    return size_t(math::absf(addedXOffset) / m_tileSize);
}

size_t
Tilemap::getRow(const real32 z) logical_const
{
    real32 addedZOffset = z - (m_nRows / 2.0f) * m_tileSize;
    return size_t(math::absf(addedZOffset) / m_tileSize);
}

size_t
Tilemap::getCols() logical_const
{
    return m_nCols;
}

size_t
Tilemap::getRows() logical_const
{
    return m_nRows;
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
    if ((col < 0 || col > m_nCols - 1) ||
        (row < 0 || row > m_nRows - 1))
    {
        return nullptr;
    }
    return m_tiles[row][col];
}

Tile*
Tilemap::getTile(const vec3f& position) bitwise_const
{
    if (position.x > m_horBounds[0] ||
        position.x < m_horBounds[1] ||
        position.z > m_verBounds[0] ||
        position.z < m_verBounds[1]) return nullptr;

    return getTile(getCol(position.x),
                   getRow(position.z));
}

void
Tilemap::renderDebug(const uint32 color,
                     const bool wireframe)
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
            
            if (m_tiles[y][x]->isSolid())
            {
                Renderer::get()->renderPrimitive(Renderer::PLANE, &debugPlane, Renderer::RED, wireframe);
            }
            else
            {
                Renderer::get()->renderPrimitive(Renderer::PLANE, &debugPlane, color == 0 ? Renderer::CYAN : Renderer::YELLOW, wireframe);                            
            }
        }
    }

#endif
}
