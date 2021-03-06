/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        entity.cpp

   File description: The implementation of the
   entity class declared in entity.h
   -------------------------------------------- */

#include "entity.h"
#include "scene.h"
#include "camera.h"
#include "tilemap.h"
#include "../util/physics.h"
#include <thread>

#define SYNC_INIT

/* -------------
   Class Methods
   ------------- */
void
Entity::bufferModel(const std::vector<cstring>& meshNames)
{
    for (auto citer = meshNames.cbegin();
              citer != meshNames.cend();
            ++citer)
    {
        Mesh m(*citer, Mesh::MESH_TYPE_NORMAL);
    }
}

/* --------------
   Public Methods
   -------------- */
Entity::Entity(const cstring               name,
               const std::vector<cstring>& meshNames,     
               const Camera*               camera,
               const Tilemap*              levelTilemap,
               Scene*                      scene,
               const uint32                entityProperties,
               const vec3f&                optPosition,     /* vec3f() */
               const cstring               optExternTexName /* nullptr*/):

               m_name(internString(name)),
               m_properties(entityProperties),               
               m_cameraRef(camera),
               m_levelTMref(levelTilemap),
               m_sceneRef(scene),
               m_targetPos(vec3f()),
               m_hasTarget(false),
               m_alive(true),
               m_enemy(false),
               m_turret(false),
               m_invisible(false)         
               
{    
    size_t nMeshes = meshNames.size();

    if(nMeshes < 2)
    {
        // Synchronous body initialization
        singleBodyInit(meshNames[0], optPosition, optExternTexName);
    }
    else
    {
        // Asynchronous body initialization
        multiBodyInit(meshNames, nMeshes, optPosition, optExternTexName);
    }

    // Add self to scene
    m_sceneRef->queueAddEntity(this);
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
    if ((m_properties & ENTITY_PROPERTY_SELECTABLE) != 0)
    {        
        setHighlighted(physics::isPicked(m_bodies[0], m_cameraRef));
    }
}

void
Entity::renderInternalComponents()
{
}

void
Entity::damage(const int32 damage)
{
    
}

Mesh*
Entity::getBody(size_t i /* 0U */) bitwise_const
{
    if (!m_alive || 
        m_bodies.size() < 1 || 
        m_bodies.size() > 5) return nullptr;

    else if (i && i < m_bodies.size()) return m_bodies[i];
    return m_bodies[0];
}


const std::vector<Mesh*>&
Entity::getBodies() logical_const
{
    return m_bodies;
}

Tile*
Entity::getTileRef(const Tilemap* tilemap) logical_const
{
    if (m_tileRefs.count(tilemap)) return m_tileRefs.at(tilemap);
    return nullptr;
}

bool
Entity::isHighlighted() logical_const
{
    return m_bodies[0]->isHighlighted();
}

bool
Entity::isAlive() logical_const
{
    return m_alive;
}

bool
Entity::isInvisible() logical_const
{
    return m_invisible;
}

bool
Entity::isEnemy() logical_const
{
    return m_enemy;
}

bool
Entity::isTurret() logical_const
{
    return m_turret;
}

void
Entity::setAlive(const bool alive)
{
    m_alive = alive;
}

void
Entity::setTileRef(const Tilemap* tilemap,
                   Tile* tileRef)
{
    m_tileRefs[tilemap] = tileRef;
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

void
Entity::setInvisible(const bool invisible)
{
    m_invisible = invisible;
}

void
Entity::setTargetPos(const vec3f& targetPos)
{
    m_targetPos = targetPos;
    m_hasTarget = true;
}

/* ---------------
   Private Methods
   --------------- */
void
Entity::singleBodyInit(const cstring meshName,
                       const vec3f&  optPosition,
                       const cstring optExternTexName)
{
    uint32 meshFlags = Mesh::MESH_TYPE_NORMAL;
    if (!optExternTexName) meshFlags |= Mesh::MESH_LOAD_SAME_TEXTURE;

    Mesh* body = new Mesh(meshName, meshFlags);

    if (optExternTexName)  body->loadNewTexture(optExternTexName);
    body->position = optPosition;
    m_bodies.push_back(body);
}

void
Entity::multiBodyInit(const std::vector<cstring>& meshNames, 
                      const size_t nMeshes,
                      const vec3f& optPosition,
                      const cstring optExternTexName)
{   

#ifndef SYNC_INIT
    std::vector<std::thread> initThreads;
#endif

    Mesh** tempMeshArray = new Mesh*[nMeshes];

    // Asynchronous component initialization
    for (size_t i = 0;
                i < nMeshes;
              ++i)         
    {

#ifndef SYNC_INIT
        initThreads.push_back(std::thread([=]()
        {
#endif
            uint32 meshFlags = Mesh::MESH_TYPE_NORMAL;
            if (!optExternTexName) meshFlags |= Mesh::MESH_LOAD_SAME_TEXTURE;

            Mesh* body = new Mesh(meshNames[i], meshFlags);

            if (optExternTexName)  body->loadNewTexture(optExternTexName);
            body->position = optPosition;

            tempMeshArray[i] = body;

#ifndef SYNC_INIT
        }));
#endif
    }   

#ifndef SYNC_INIT
    for (auto iter = initThreads.begin();
              iter != initThreads.end();
            ++iter)
    {
        iter->join();
    }
#endif
    // Assign to final body vector
    m_bodies.assign(tempMeshArray, tempMeshArray + nMeshes);
    delete tempMeshArray;
}