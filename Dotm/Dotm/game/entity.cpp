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
#include "pathfinding.h"
#include "command.h"
#include "../util/physics.h"
#include "../util/logging.h"

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
               m_cameraRef(camera),
               m_targetPos(vec3f()),
               m_hasTarget(false)
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

    for (auto iter = m_path.begin();
              iter != m_path.end();
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

        if (m_hasTarget)
        {            
            uint32 goalAccum  = 0U;
            uint32 nBodies    = m_bodies.size();

            for (auto iter = m_bodies.begin();
                      iter != m_bodies.end();
                    ++iter)
            {
                goalAccum += math::lerpf((*iter)->position.x, m_targetPos.x, 0.1f);
                goalAccum += math::lerpf((*iter)->position.y, m_targetPos.y, 0.1f);
                goalAccum += math::lerpf((*iter)->position.z, m_targetPos.z, 0.1f);
            }
            
            // Lerp on goal returns 1, so the entity has reached 
            // the target when all of its bodies' positional coordinates
            // have reached the goal and have returned 1 on lerp. And hence nBodies * 3 (x, y, z)
            if(goalAccum == nBodies * 3) 
            {
                // Atomic path update
                m_entityMutex.lock();
                if (m_path.size() > 0)
                {                    
                    m_path.front()->execute();
                    m_path.pop_front();
                }
                else
                {
                    m_hasTarget = false;
                }
                m_entityMutex.unlock();
            }
        }        
    }

    // if selectable
    if ((m_properties & ENTITY_PROPERTY_SELECTABLE) != 0)
    {        
        setHighlighted(physics::isPicked(m_bodies[0], m_cameraRef));
    }
}

void
Entity::findPathTo(const vec3f&   target, 
                   const Tilemap* grid,
                   const bool     erasePrevious)
{      
    m_aiThread = std::thread([=]()
    {
        std::list<Command*> newInstructions;
        pathfinding::findPath(grid, 
                              grid->getTile(m_bodies[0]->position),
                              grid->getTile(target), 
                              this,
                              &newInstructions);

        // Atomic path update
        m_entityMutex.lock();
        m_hasTarget = true;

        // Erase previous path with new contents
        if (erasePrevious)
        {
            for (auto iter = m_path.begin();
                      iter != m_path.end();
                    ++iter)
            {
                delete *iter;
            }
            
            m_path.assign(newInstructions.begin(),
                          newInstructions.end());
        }
        // Keep previous content and add new at the end
        else
        {
            m_path.insert(m_path.end(),
                          newInstructions.begin(),
                          newInstructions.end());
        }

        m_path.front()->execute();
        m_path.pop_front();
        m_entityMutex.unlock();        
    });
    m_aiThread.detach();
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

const vec3f&
Entity::getTargetPos() logical_const
{
    return m_targetPos;
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

void
Entity::setTargetPos(const vec3f& targetPos)
{
    m_targetPos = targetPos;
    m_hasTarget = true;
}