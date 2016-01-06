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
class Mesh;
class Light;
class Scene
{
public:

    typedef std::vector<const Mesh*>::const_iterator   mesh_citer;
    typedef std::vector<const Light*>::const_iterator  light_citer;
    typedef std::vector<Entity*>::const_iterator entity_citer;

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

    void
    requestLightIter(light_citer& outBegin,
                     light_citer& outEnd) logical_const;
    
    void
    requestMeshIter(mesh_citer& outBegin,
                    mesh_citer& outEnd) logical_const;
    void
    requestEntityIter(entity_citer& outBegin,
                      entity_citer& outEnd) logical_const;

    void
    addEntity(Entity* entity);

    void
    addMesh(const Mesh* mesh);

    void
    addLight(const Light* light);

    void
    removeEntity(Entity* entity);

    void
    removeMesh(const Mesh* mesh);

    void
    removeLight(const Light* light);

private:

    std::vector<Entity*> m_entities;
    std::vector<const Mesh*>   m_meshes;
    std::vector<const Light*>  m_lights;

};