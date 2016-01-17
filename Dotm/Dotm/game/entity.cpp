/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        mesh.cpp

   File description: The implementation of the
   entity class declared in entity.h
   -------------------------------------------- */

#include "entity.h"
#include "scene.h"
#include "camera.h"
#include "tilemap.h"
#include "../util/physics.h"
#include <thread>

/* --------------
   Public Methods
   -------------- */
Entity::Entity(const cstring               name,
               const std::vector<cstring>& meshNames,     
               Camera*                     camera,
               const uint32                entityProperties,
               const vec3f&                position,     /* vec3f() */
               const cstring               externTexName /* nullptr*/):

               m_name(internString(name)),
               m_properties(entityProperties),               
               m_cameraRef(camera)
{

    std::vector<std::thread> initThreads;
    size_t nMeshes = meshNames.size();
    Mesh** tempMeshArray = new Mesh*[nMeshes];

    // Asynchronous component initialization
    for (size_t i = 0;
                i < nMeshes;
              ++i)         
    {
        initThreads.push_back(std::thread([=]()
        {
            uint32 meshFlags = Mesh::MESH_TYPE_NORMAL;
            if (!externTexName) meshFlags |= Mesh::MESH_LOAD_SAME_TEXTURE;

            Mesh* body = new Mesh(meshNames[i], meshFlags);

            if (externTexName)  body->loadNewTexture(externTexName);
            body->position = position;

            tempMeshArray[i] = body;
        }));
    }   

    for (auto iter = initThreads.begin();
              iter != initThreads.end();
            ++iter)
    {
        iter->join();
    }

    // Assign to final body vector
    m_bodies.assign(tempMeshArray, tempMeshArray + nMeshes);
    delete tempMeshArray;
}

Entity::~Entity()
{
    for (auto iter = m_bodies.begin();
              iter != m_bodies.end();
            ++iter) 
    {
        delete *iter;
    }
}

void
Entity::update()
{    
    // if not static entity
    if ((m_properties & ENTITY_PROPERTY_STATIC) == 0) 
    {
        m_bodies[0]->rotation.y += 0.01f;
    }

    // if selectable
    if ((m_properties & ENTITY_PROPERTY_SELECTABLE) != 0)
    {        
        setHighlighted(physics::isPicked(m_bodies[0], m_cameraRef));
    }
}

Mesh*
Entity::getBody(size_t i /* 0U */) bitwise_const
{
    if (i && i < m_bodies.size()) return m_bodies[i];
    return m_bodies[0];
}

bool
Entity::isHighlighted() logical_const
{
    return m_bodies[0]->isHighlighted();
}

const std::vector<Mesh*>&
Entity::getBodies() logical_const
{
    return m_bodies;
}

Tile*
Entity::getTileRef() logical_const
{
    return m_tileRef;
}

void
Entity::setTileRef(Tile* tileRef)
{
    m_tileRef = tileRef;
}

void
Entity::setHighlighted(const bool highlighted)
{
    for (auto iter = m_bodies.begin();
              iter != m_bodies.end();
            ++iter)
    {
        (*iter)->setHighlighted(highlighted);
    }
}