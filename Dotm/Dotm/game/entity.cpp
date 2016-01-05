/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        mesh.cpp

   File description: The implementation of the
   entity class declared in entity.h
   -------------------------------------------- */

#include "entity.h"
#include "scene.h"
#include <thread>
#include <mutex>

// Entity creation mutex
std::mutex mutex;

Entity::Entity(const cstring               name,
               const std::vector<cstring>& meshNames,
               Scene*                      scene,
               const vec3f&                position /* vec3f() */):

               m_name(internString(name)),
               m_sceneRef(scene)
{
    std::vector<std::thread> initThreads;

    for (auto citer = meshNames.cbegin();
         citer != meshNames.cend();
         ++citer)
    {
        initThreads.push_back(std::thread([=]()
        {
            Mesh* body = new Mesh(*citer,
                                   Mesh::MESH_TYPE_NORMAL | Mesh::MESH_LOAD_SAME_TEXTURE,
                                   scene);

            body->position = position;

            std::lock_guard<std::mutex> lock(mutex);
            m_bodies.push_back(body);
        }));
    }

    for (auto iter = initThreads.begin();
         iter != initThreads.end();
         ++iter) iter->join();

    m_sceneRef->addEntity(this);
}

Entity::~Entity()
{
    for (auto iter = m_bodies.begin();
         iter != m_bodies.end();
         ++iter) delete *iter;

    m_sceneRef->removeEntity(this);
}

void
Entity::update()
{

}

const std::vector<Mesh*>&
Entity::getBodies() logical_const
{
    return m_bodies;
}