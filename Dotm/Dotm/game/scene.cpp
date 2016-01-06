/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        scene.cpp

   File description: Implementation of the scene
   class declared in scene.h
   ------------------------------------------------ */

#include "scene.h"
#include "entity.h"
#include <algorithm>

/* --------------
   Public Methods
   -------------- */
Scene::Scene()
{

}

Scene::~Scene()
{
    clear();
}

void
Scene::update()
{
    for (auto iter = m_entities.begin();
        iter != m_entities.end();
        ++iter) 
    {
        (*iter)->update();
    }
}

void
Scene::clear()
{
    m_entities.clear();
    m_lights.clear();
    m_meshes.clear();
}

void
Scene::requestLightIter(Scene::light_citer& outBegin,
                        Scene::light_citer& outEnd) logical_const
{
    outBegin = m_lights.cbegin();
    outEnd   = m_lights.cend();
}
    
void
Scene::requestMeshIter(Scene::mesh_citer& outBegin,
                       Scene::mesh_citer& outEnd) logical_const
{
    outBegin = m_meshes.cbegin();
    outEnd   = m_meshes.cend();
}

void
Scene::requestEntityIter(Scene::entity_citer& outBegin,
                         Scene::entity_citer& outEnd) logical_const
{
    outBegin = m_entities.cbegin();
    outEnd   = m_entities.cend();
}

void
Scene::addEntity(Entity* entity)
{
    m_entities.push_back(entity);
}

void
Scene::addMesh(const Mesh* mesh)
{
    m_meshes.push_back(mesh);
}

void
Scene::addLight(const Light* light)
{
    m_lights.push_back(light);
}

void
Scene::removeEntity(Entity* entity)
{
    for (size_t i = 0; i < m_entities.size(); ++i)
    {
        if (m_entities[i] == entity) m_entities.erase(m_entities.begin() + i);
    }
}

void
Scene::removeMesh(const Mesh* mesh)
{
    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        if (m_meshes[i] == mesh) m_meshes.erase(m_meshes.begin() + i);
    }
}

void
Scene::removeLight(const Light* light)
{
    for (size_t i = 0; i < m_lights.size(); ++i)
    {
        if (m_lights[i] == light) m_lights.erase(m_lights.begin() + i);
    }
}