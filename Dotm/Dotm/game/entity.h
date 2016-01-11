/* --------------------------------------------
   Author:           Alex Koukoulas
   Date:             5/1/2016
   File name:        mesh.h

   File description: An abstract representation
   of a concrete game object both playable and
   non playable.
   -------------------------------------------- */

#pragma once
#include "../dotmdef.h"
#include "../util/strings.h"
#include "../rendering/mesh.h"
#include "../util/math.h"
#include <vector>

class Scene;
class Camera;
class Entity
{
public:

    typedef std::vector<Mesh*>::const_reverse_iterator body_iter;

public:

    static const uint32 ENTITY_PROPERTY_SELECTABLE = 0x01;
    static const uint32 ENTITY_PROPERTY_STATIC     = 0x02;
    
public:

    Entity(const cstring               name,
           const std::vector<cstring>& meshNames,
           Scene*                      scene,
           Camera*                     camera,
           const uint32                entityProperties,
           const vec3f&                optPosition = vec3f(),
           const cstring               externTexName = nullptr);

    virtual
    ~Entity();

    virtual void
    update();

    Mesh*
    getBody(size_t i = 0U) bitwise_const;
    
    bool
    isHighlighted() logical_const;

    void
    acquireBodies(Entity::body_iter& begin,
                  Entity::body_iter& end) logical_const;

private:

    const stringID     m_name;
    const Camera*      m_cameraRef;
    uint32             m_properties;
    std::vector<Mesh*> m_bodies;
    Scene*             m_sceneRef;
};