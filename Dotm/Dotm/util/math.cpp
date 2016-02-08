/* --------------------------------------------------
   Author:           Alex Koukoulas
   Date:             3/1/2016
   File name:        math.cpp

   File description: The necessary implementations of 
   the math classes declared in math.h
   -------------------------------------------------- */

#include "math.h"

/* ===============
   Class: Geometry
   =============== */

/* --------------
   Public Methods
   -------------- */

math::Geometry::Geometry(const vec3f& position):

                         m_position(position)
{

}

math::Geometry::~Geometry(){}

const vec3f&
math::Geometry::getPosition() logical_const
{
    return m_position;
}

void
math::Geometry::setPosition(const vec3f& position)
{
    m_position = position;
}




/* ==============
   Class: Frustum
   ============== */

/* --------------
   Public Methods
   -------------- */
math::Frustum::Frustum():

    Geometry(D3DXVECTOR3())
{

}

math::Frustum::~Frustum(){}

const plane&
math::Frustum::getPlane(const size_t index) logical_const
{
    return m_planes[index];
}

void
math::Frustum::setPlane(const size_t index,
                        const plane& plane)
{
    m_planes[index] = plane;
}

/* ==========
   Class: Ray
   ========== */

/* --------------
   Public Methods
   -------------- */
math::Ray::Ray():

    Geometry(vec3f()),
    m_direction()
{
}

math::Ray::Ray(const vec3f& position,
               const vec3f& direction):
    
               Geometry(position),
               m_direction(direction)
{

}

math::Ray::~Ray(){}

const vec3f&
math::Ray::getDirection() logical_const
{
    return m_direction;
}

void
math::Ray::setDirection(const vec3f& direction)
{
    m_direction = direction;
}


/* =============
   Class: Sphere
   ============= */

/* --------------
   Public Methods
   -------------- */
math::Sphere::Sphere(const vec3f& position,
                     const real32 radius):
    
                     Geometry(position),
                     m_radius(radius)
{

}

math::Sphere::~Sphere(){}

const real32
math::Sphere::getRadius() logical_const
{
    return m_radius;
}

void
math::Sphere::setRadius(const real32 radius)
{
    m_radius = radius;
}


/* ===========
   Class: Cube
   =========== */

/* --------------
   Public Methods
   -------------- */
math::Cube::Cube(const vec3f& position,
                 const vec3f& dimensions):

                 Geometry(position),
                 m_dimensions(dimensions)
{

}

math::Cube::~Cube(){}

const vec3f&
math::Cube::getDimensions() logical_const
{
    return m_dimensions;
}
   
void
math::Cube::setDimensions(const vec3f& dimensions)
{
    m_dimensions = dimensions;
}

/* ============
   Class: Plane
   ============ */

/* ------------- 
   Public Method
   ------------- */
math::GeoPlane::GeoPlane(const vec3f& position,
                         const vec2f& dimensions):

                         Geometry(position),
                         m_dimensions(dimensions)
{

}

math::GeoPlane::~GeoPlane(){};

const vec2f&
math::GeoPlane::getDimensions() logical_const
{
    return m_dimensions;
}

void
math::GeoPlane::setDimensions(const vec2f& dimensions)
{
    m_dimensions = dimensions;
}

   