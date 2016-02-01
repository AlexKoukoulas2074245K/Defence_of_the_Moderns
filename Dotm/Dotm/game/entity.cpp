/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        mesh.cpp

   File description: The implementation of the
   entity class and subclasses declared in 
   entity.h
   -------------------------------------------- */

#include "entity.h"
#include "scene.h"
#include "camera.h"
#include "tilemap.h"
#include "pathfinding.h"
#include "command.h"
#include "../util/physics.h"
#include "../util/logging.h"

/* =============
   Class: Entity
   ============= */

/* --------------
   Public Methods
   -------------- */
Entity::Entity(const cstring               name,
               const std::vector<cstring>& meshNames,     
               const Camera*               camera,
               const uint32                entityProperties,
               const vec3f&                optPosition,     /* vec3f() */
               const cstring               optExternTexName /* nullptr*/):

               m_name(internString(name)),
               m_properties(entityProperties),               
               m_cameraRef(camera),
               m_targetPos(vec3f()),
               m_hasTarget(false),
               m_alive(true)
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
            if (!optExternTexName) meshFlags |= Mesh::MESH_LOAD_SAME_TEXTURE;

            Mesh* body = new Mesh(meshNames[i], meshFlags);

            if (optExternTexName)  body->loadNewTexture(optExternTexName);
            body->position = optPosition;

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
Entity::setTargetPos(const vec3f& targetPos)
{
    m_targetPos = targetPos;
    m_hasTarget = true;
}


/* ====================
   Class: AIEntity
   ==================== */

/* --------------
   Public Methods
   -------------- */

AIEntity::AIEntity(const cstring               name,
                   const std::vector<cstring>& meshNames,           
                   const Camera*               camera,
                   const bool                  optSelectable,   /* true    */
                   const vec3f&                optPosition,     /*vec3f()  */
                   const vec3f&                optVelocity,     /* vec3f() */
                   const cstring               optExternTexName /* nullptr */):

                   Entity(name,
                          meshNames,
                          camera,
                          optSelectable ? ENTITY_PROPERTY_SELECTABLE : 0U,
                          optPosition, 
                          optExternTexName),

                   m_velocity(optVelocity)
{
}

AIEntity::~AIEntity()
{
    for (auto iter = m_path.begin();
        iter != m_path.end();
        ++iter)
    {
        delete *iter;
    }
}

void
AIEntity::update()
{
    //m_bodies[0]->rotation.y += 0.01f;

    if (m_hasTarget)
    {
        uint32 goalAccum = 0U;
        uint32 nBodies = m_bodies.size();

        for (auto iter = m_bodies.begin();
                  iter != m_bodies.end();
                ++iter)
        {
            goalAccum += math::lerpf((*iter)->position.x, m_targetPos.x, m_velocity.x);
            goalAccum += math::lerpf((*iter)->position.y, m_targetPos.y, m_velocity.y);
            goalAccum += math::lerpf((*iter)->position.z, m_targetPos.z, m_velocity.z);
        }

        // Lerp on goal returns 1, so the entity has reached 
        // the target when all of its bodies' positional coordinates
        // have reached the goal and have returned 1 on lerp. And hence nBodies * 3 (x, y, z)
        if (goalAccum == nBodies * 3)
        {
            // Atomic path update
            m_pathMutex.lock();
            if (m_path.size() > 0)
            {
                m_path.front()->execute();
                m_path.pop_front();
            }
            else
            {
                m_hasTarget = false;
            }
            m_pathMutex.unlock();
        }
    }

    Entity::update();
}

void
AIEntity::findPathTo(const vec3f&   target, 
                     const Tilemap* grid,
                     const bool     erasePrevious)
{      
    m_pathThread = std::thread([=]()
    {
        std::list<Command*> newInstructions;
        pathfinding::findPath(grid, 
                              grid->getTile(m_bodies[0]->position),
                              grid->getTile(target), 
                              this,
                              &newInstructions);

        // Atomic path update
        m_pathMutex.lock();
        
        // Replace previous commands in the path with new ones
        if (erasePrevious)
        {
            for (auto iter = m_path.begin();
                      iter != m_path.end();
                    ++iter)
            {
                delete *iter;
            }
            
            m_path.clear();

            for (auto citer = newInstructions.cbegin();
                      citer != newInstructions.cend();
                    ++citer)
            {
                m_path.push_back(*citer);
            }            
        }
        // Keep previous content and add new commands at the end
        else
        {
            for (auto citer = newInstructions.cbegin();
                      citer != newInstructions.cend();
                    ++citer)
            {
                m_path.push_back(*citer);
            }            
        }

        if (m_path.size() > 0)
        {
            m_hasTarget = true;
            m_path.front()->execute();
            m_path.pop_front();
        }
        m_pathMutex.unlock();        
    });
    m_pathThread.detach();
}


