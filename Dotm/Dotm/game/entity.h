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
class Entity
{
public:

    Entity(const cstring               name,
           const std::vector<cstring>& meshNames,
           Scene*                      scene,
           const vec3f&                optPosition = vec3f());

    virtual
    ~Entity();

    virtual void
    update();

    const std::vector<Mesh*>&
    getBodies() logical_const;

private:

    const stringID     m_name;
    std::vector<Mesh*> m_bodies;
    Scene*             m_sceneRef;
};