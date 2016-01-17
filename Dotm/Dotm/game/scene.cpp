/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        scene.cpp

   File description: Implementation of the scene
   class declared in scene.h
   ------------------------------------------------ */


#include "scene.h"
#include "entity.h"
#include "tilemap.h"
#include <algorithm>
#include "../util/logging.h"

#define SPPART

/* ---------
   Constants
   --------- */
const real32 Scene::SCENE_CELL_SIZE = 10.0f;
const uint32 Scene::SCENE_NUM_CELLS = 10U;
   
/* --------------
   Public Methods
   -------------- */
Scene::Scene():
    m_entityGraph(new Tilemap(SCENE_NUM_CELLS, 
                              SCENE_NUM_CELLS, 
                              SCENE_CELL_SIZE, 
                              {0.0f, 2.0f, 0.0f}))
{
    
}

Scene::~Scene()
{
    clear();
    delete m_entityGraph;
}

void
Scene::update()
{
    for (auto iter = m_cachedEntities.begin();
              iter != m_cachedEntities.end();
            ++iter)
    {
        // Update entity
        (*iter)->update();
    
        Tile* oldTile    = (*iter)->getTileRef();
        Tile* targetTile = m_entityGraph->getTile(
            m_entityGraph->getCol((*iter)->getBody()->position.x),
            m_entityGraph->getRow((*iter)->getBody()->position.z));

        // Tile update
        if (targetTile != oldTile)
        {
            for (size_t i = 0;
                        i < oldTile->t_entities.size();
                      ++i)
            {
                if (oldTile->t_entities[i] == (*iter)) 
                {
                    oldTile->t_entities.erase(oldTile->t_entities.begin() + i);
                    break;
                }
            }
            targetTile->t_entities.push_back(*iter);
            (*iter)->setTileRef(targetTile);
        }
    }
}

void
Scene::clear()
{
    m_cachedEntities.clear();
    m_lights.clear();    
}

const std::vector<const Light*>&
Scene::getLights() logical_const
{
    return m_lights;
}

const std::vector<Entity*>&
Scene::getEntities() logical_const
{
    m_entityGraph->renderDebug();
    return m_cachedEntities;    
}

Entity*
Scene::getHighlightedEntity() bitwise_const
{
    Entity* highlighted = nullptr;
    
    for (auto iter = m_cachedEntities.begin();
              iter != m_cachedEntities.end();
            ++iter)
    {
        if ((*iter)->isHighlighted() && !highlighted)
        {
            highlighted = (*iter);
        }
        else
        {
            (*iter)->setHighlighted(false);
        }
    }

    return highlighted;
}

void
Scene::addEntity(Entity* entity)
{
    Tile* targetTile = m_entityGraph->getTile(
        m_entityGraph->getCol(entity->getBody()->position.x),
        m_entityGraph->getRow(entity->getBody()->position.z));

    targetTile->t_entities.push_back(entity);
    entity->setTileRef(targetTile);

    m_cachedEntities.push_back(entity);
}


void
Scene::addLight(const Light* light)
{
    m_lights.push_back(light);
}

void
Scene::removeEntity(Entity* entity)
{
    Tile* targetTile = entity->getTileRef();

    for (size_t i = 0;
                i < targetTile->t_entities.size();
              ++i)
    {
        if (targetTile->t_entities[i] == entity) 
        {
            targetTile->t_entities.erase(targetTile->t_entities.begin() + i);
            break;
        }
    } 

    for (size_t i = 0;
                i < m_cachedEntities.size();
              ++i)
    {
        if (m_cachedEntities[i] == entity)
        {
            m_cachedEntities.erase(m_cachedEntities.begin() + i);
            break;
        }
    }
}

void
Scene::removeLight(const Light* light)
{
    for (size_t i = 0;
                i < m_lights.size();
              ++i)
    {
        if (m_lights[i] == light) m_lights.erase(m_lights.begin() + i);
        break;
    }
}