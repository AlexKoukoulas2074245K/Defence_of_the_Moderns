/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        mesh.h

   File description: An abstract representation
   of a concrete game object both playable and
   non playable along with the class declarations
   that inherit from the base class.
   ---------------------------------------------- */

#pragma once
#include "../dotmdef.h"
#include "../util/strings.h"
#include "../rendering/mesh.h"
#include "../util/math.h"
#include <vector>
#include <list>
#include <unordered_map>
#include <mutex>
#include <thread>

struct Tile;
class  Scene;
class  Camera;
class  Tilemap;
class  Command;

/* =============
   Class: Entity
   ============= */
class  Entity
{
public:

    typedef std::vector<Mesh*>::const_reverse_iterator body_iter;

public:

    static const uint32 ENTITY_PROPERTY_SELECTABLE = 0x01;
    static const uint32 ENTITY_PROPERTY_STATIC     = 0x02;
    
public:

    Entity(const cstring               name,
           const std::vector<cstring>& meshNames,           
           const Camera*               camera,
           Scene*                      scene,
           const uint32                entityProperties,
           const vec3f&                optPosition = vec3f(),
           const cstring               optExternTexName = nullptr);

    virtual
    ~Entity();

    virtual void
    update();

    virtual void
    damage(const int32 damage);

    Mesh*
    getBody(size_t i = 0U) bitwise_const;
   
    const std::vector<Mesh*>&
    getBodies() logical_const;

    Tile*
    getTileRef(const Tilemap* tilemap) logical_const;
    
    bool
    isHighlighted() logical_const;

    bool
    isAlive() logical_const;

    bool
    isEnemy() logical_const;

    void
    setAlive(const bool alive);

    void
    setTileRef(const Tilemap* tilemap,
               Tile* tile); 
       
    void
    setHighlighted(const bool highlighted);

    void
    setTargetPos(const vec3f& targetPos);

protected:

    const stringID     m_name;
    const Camera*      m_cameraRef;
    Scene*             m_sceneRef;    
    uint32             m_properties;
    std::vector<Mesh*> m_bodies;            
    vec3f              m_targetPos;
    bool               m_hasTarget;
    bool               m_alive;
    bool               m_enemy;
    int32              m_stamina;

    // M-N relationship here. A tile can have many entities resident, 
    // and an entity be resident in many different tilemaps. Thats why
    // a map of tile references must be kept.
    std::unordered_map<const Tilemap*, Tile*> m_tileRefs;  

};


/* ====================
   Class: AIEntity
   ==================== */
class AIEntity: public Entity
{

public:

    AIEntity(const cstring               name,
             const std::vector<cstring>& meshNames,           
             const Camera*               camera,
             Scene*                      scene,
             const bool                  optSelectable = true,
             const vec3f&                optPosition = vec3f(),
             const vec3f&                optVelocity = vec3f(),
             const cstring               optExternTexName = nullptr);

    virtual
    ~AIEntity();
    
    virtual void
    update() override;

    virtual void
    damage(const int32 damage) override;

    void
    findPathTo(const vec3f&   target, 
               const Tilemap* grid,
               const bool     erasePrevious);    

private:

    std::list<Command*> m_path;
    std::thread         m_pathThread;  
    std::mutex          m_pathMutex;
    vec3f               m_velocity;
    
};