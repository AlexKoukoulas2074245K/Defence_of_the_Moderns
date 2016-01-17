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

struct Tile;
class  Scene;
class  Camera;
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
   
    const std::vector<Mesh*>&
    getBodies() logical_const;

    Tile*
    getTileRef() logical_const;

    void
    setTileRef(Tile* tileRef); 
    
    void
    setHighlighted(const bool highlighted);

private:

    const stringID     m_name;
    const Camera*      m_cameraRef;
    Tile*              m_tileRef;
    uint32             m_properties;
    std::vector<Mesh*> m_bodies;

};