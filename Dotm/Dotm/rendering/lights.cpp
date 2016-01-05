/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        lights.cpp

   File description: Implementation of all the
   light classes declared in lights.h
   ------------------------------------------------ */

#include "lights.h"
#include "../game/scene.h"

/* ============
   class: Light
   ============ */

/* --------------
   Public Methods
   -------------- */

Light::Light(const vec4f&    ambientCol,
             const vec4f&    diffuseCol,
             const LightType lightType,
             Scene*          sceneptr):

             m_ambientColor(ambientCol),
             m_diffuseColor(diffuseCol),
             m_lightType(lightType),
             m_sceneRef(sceneptr)
{
    if (m_sceneRef) m_sceneRef->addLight(this);
}

Light::~Light()
{
    if(m_sceneRef) m_sceneRef->removeLight(this);
}

Light::LightType
Light::getType() logical_const
{
    return m_lightType;
}

const vec4f&
Light::getAmbientColor() logical_const
{
    return m_ambientColor;
}

const vec4f&
Light::getDiffuseColor() logical_const
{
    return m_diffuseColor;
}

/* =======================
   Class: DirectionalLight
   ======================= */

/* --------------
   Public Methods
   -------------- */
DirectionalLight::DirectionalLight(const vec4f& ambientCol,
                                   const vec4f& diffuseCol,
                                   const vec3f& direction,
                                   Scene*       sceneptr):

                                   Light(ambientCol, diffuseCol, LIGHT_DIRECTIONAL, sceneptr),
                                   m_direction(direction)
{

}

DirectionalLight::~DirectionalLight(){}

const vec3f&
DirectionalLight::getDirection() logical_const
{
    return m_direction;
}


/* =================
   class: PointLight
   ================= */

/* --------------
   Public Methods
   -------------- */
PointLight::PointLight(const vec4f& ambientCol,
                       const vec4f& diffuseCol,
                       const vec3f& position,
                       const real32 range,
                       Scene*       sceneptr):

                       Light(ambientCol, diffuseCol, LIGHT_POINT, sceneptr),
                       m_position(position),
                       m_range(range)                    
{
    
}

PointLight::~PointLight(){}

const vec3f&
PointLight::getPosition() logical_const
{
    return m_position;
}

const real32
PointLight::getRange() logical_const
{
    return m_range;
}