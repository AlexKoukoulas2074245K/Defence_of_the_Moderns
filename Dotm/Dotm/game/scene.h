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
    typedef std::vector<const Entity*>::const_iterator entity_citer;

public:

    static Scene*
    get();    

    ~Scene();

    Scene(const Scene& rhs) = delete;

    Scene&
    operator = (const Scene& rhs) = delete;
    
    void
    clearScene();

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
    addEntity(const Entity* entity);

    void
    addMesh(const Mesh* mesh);

    void
    addLight(const Light* light);

    void
    removeEntity(const Entity* entity);

    void
    removeMesh(const Mesh* mesh);

    void
    removeLight(const Light* light);

private:

    Scene();

private:

    std::vector<const Entity*> m_entities;
    std::vector<const Mesh*>   m_meshes;
    std::vector<const Light*>  m_lights;
};