/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        mesh.cpp

   File description: The implementation of the
   entity class declared in entity.h
   -------------------------------------------- */

#define _THREADED_INIT 1

#include "entity.h"
#include "scene.h"
#include <thread>
#include <mutex>

// Entity creation mutex
std::mutex mutex;

Entity::Entity(const cstring               name,
               const std::vector<cstring>& meshNames,
               Scene*                      scene,
               const vec3f&                position,     /* vec3f() */
               const cstring               externTexName /* nullptr*/):

               m_name(internString(name)),
               m_sceneRef(scene)
{
#if _THREADED_INIT
    std::vector<std::thread> initThreads;
#endif

    for (auto citer = meshNames.cbegin();
         citer != meshNames.cend();
         ++citer)
    {

#if _THREADED_INIT
        initThreads.push_back(std::thread([=]()
        {
#endif
            uint32 meshFlags = Mesh::MESH_TYPE_NORMAL;
            if (!externTexName) meshFlags |= Mesh::MESH_LOAD_SAME_TEXTURE;

            Mesh* body = new Mesh(*citer, meshFlags, scene);

            if (externTexName) 
                body->loadNewTexture(externTexName);
            body->position = position;

#if _THREADED_INIT
            std::lock_guard<std::mutex> lock(mutex);
#endif
            m_bodies.push_back(body);
#if _THREADED_INIT
        }));
#endif
    }

#if _THREADED_INIT
    for (auto iter = initThreads.begin();
         iter != initThreads.end();
         ++iter) iter->join();
#endif

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
    //m_bodies[0]->rotation.y += 0.01f;
}

Mesh*
Entity::getBody(size_t i /* 0U */) bitwise_const
{
    if (i && i < m_bodies.size()) return m_bodies[i];
    return m_bodies[0];
}

const std::vector<Mesh*>&
Entity::getBodies() logical_const
{
    return m_bodies;
}