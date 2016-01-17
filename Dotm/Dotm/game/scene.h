/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        scene.h

   File description: A class containing all the 
   entities taking place in the game.
   ------------------------------------------------ */

#pragma once

#include <vector>
#include "../util/strings.h"
#include "../dotmdef.h"

class Entity;
class Light;
class Tilemap;
class Scene
{
public:

    static const uint32 SCENE_NUM_CELLS;
    static const real32 SCENE_CELL_SIZE;

public:
    
    Scene();

    ~Scene();

    Scene(const Scene& rhs) = delete;

    Scene&
    operator = (const Scene& rhs) = delete;
    
    void
    update();

    void
    clear();
    
    const std::vector<const Light*>&
    getLights() logical_const;

    const std::vector<Entity*>&
    getEntities() logical_const;

    Entity*
    getHighlightedEntity() bitwise_const;

    void
    addEntity(Entity* entity);

    void
    addLight(const Light* light);

    void
    removeEntity(Entity* entity);

    void
    removeLight(const Light* light);

private:
    
    std::vector<const Light*> m_lights;
    std::vector<Entity*>      m_cachedEntities;
    Tilemap*                  m_entityGraph;    
};