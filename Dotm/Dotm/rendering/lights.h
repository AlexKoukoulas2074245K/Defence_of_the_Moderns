/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        lights.h

   File description: A class encapsulating the 
   properties of the different lights present in 
   the game.
   ------------------------------------------------ */

#pragma once

#include "../util/math.h"
#include "../dotmdef.h"

/* ============
   class: Light
   ============ */
class Scene;
class Light
{
public:

    enum LightType
    {
        LIGHT_DIRECTIONAL,
        LIGHT_POINT,
        LIGHT_SPOT
    };

public:

    Light(const vec4f&    ambientCol,
          const vec4f&    diffuseCol,
          const LightType lightType,
          Scene*          sceneptr);

    virtual
    ~Light();

    LightType
    getType() logical_const;

    const vec4f&
    getAmbientColor() logical_const;

    const vec4f&
    getDiffuseColor() logical_const;

protected:

    LightType m_lightType;
    vec4f     m_ambientColor;
    vec4f     m_diffuseColor;
    Scene*    m_sceneRef;

};

/* =======================
   class: DirectionalLight
   ======================= */
class DirectionalLight: public Light
{
public:

    DirectionalLight(const vec4f& ambientCol,
                     const vec4f& diffuseCol,
                     const vec3f& direction,
                     Scene*       sceneptr);

    ~DirectionalLight();

    const vec3f&
    getDirection() logical_const;

private:

    vec3f m_direction;

};

/* =================
   class: PointLight
   ================= */
class PointLight: public Light
{
public:

    PointLight(const vec4f& ambientCol,
               const vec4f& diffuseCol,
               const vec3f& position,
               const real32 range,
               Scene*       sceneptr);

    ~PointLight();

    const vec3f&
    getPosition() logical_const;

    const real32
    getRange() logical_const;

private:

    vec3f  m_position;
    real32 m_range;
    
};