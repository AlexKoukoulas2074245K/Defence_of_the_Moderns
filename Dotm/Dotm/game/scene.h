/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        scene.h

   File description: A class containing all the 
   entities taking place in the game.
   ------------------------------------------------ */

#pragma once

#include <vector>
#include <queue>
#include "../util/strings.h"
#include "../dotmdef.h"

class Entity;
class Light;
class Tilemap;
class Scene
{
public:

    static const real32 SCENE_CELL_SIZE;
    static const uint32 SCENE_HOR_NUM_CELLS;
    static const uint32 SCENE_VER_NUM_CELLS;

public:
    
    Scene();

    ~Scene();

    Scene(const Scene& rhs) = delete;

    Scene&
    operator = (const Scene& rhs) = delete;
    
    void
    update();

    void
    clearScene();
    
    const std::vector<const Light*>&
    getLights() logical_const;

    const std::vector<Entity*>&
    getEntities() logical_const;

    const Tilemap*
    getInternalTilemap() logical_const;

    Entity*
    getHighlightedEntity() bitwise_const;

    // <summary>
    // <para>
    // From the moment an entity is added to the scene
    // the pointer ownership moves to the scene
    // </para>
    // </summary>
    void queueAddEntity(Entity* entity);

    // <summary>
    // <para>
    // Adds the entity to the kill queue
    // the scene is responsible for deleting the entity
    // </para>
    // </summary>
    void queueKillEntity(Entity* entity);

    void
    addLight(const Light* light);

    void
    removeLight(const Light* light);

private:

    void
    addEntity(Entity* entity);

    void
    removeEntity(Entity* entity);

private:
    
    std::vector<const Light*> m_lights;
    std::vector<Entity*>      m_cachedEntities;
    std::queue<Entity*>       m_waitToAddEntities;
    std::queue<Entity*>       m_waitToKillEntities;
    Tilemap*                  m_entityGraph;    
};