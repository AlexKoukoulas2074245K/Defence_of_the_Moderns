/* --------------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        math.h

   File description: A header file containing all
   the necessary linking and file inclusion to use
   the specified math structures and functions along
   with helpful math classes and functions to be used
   throughout the project.
   -------------------------------------------------- */

#pragma once

// Necessary link to d3dx10
#pragma comment(lib, "d3dx10.lib")

// d3dx10 header inclusion
#include <d3dx10.h>

#include "../dotmdef.h"

#define PI_FL ((float) D3DX_PI)

typedef D3DXMATRIX  mat4x4;
typedef D3DXVECTOR2 vec2f;
typedef D3DXVECTOR3 vec3f;
typedef D3DXVECTOR4 vec4f;
typedef D3DXPLANE   plane;

namespace math
{
    inline vec4f
    getVec4f(const vec3f& in) { return vec4f(in.x, in.y, in.z, 0.0f); }

    inline vec3f
    getVec3f(const vec4f& in) { return vec3f(in.x, in.y, in.z); }

    inline vec3f
    getVec3f(const vec2f& in) { return vec3f(in.x, 0.0f, in.y); }

    inline vec2f
    getVec2f(const vec3f& in) { return vec2f(in.x, in.y); }

    inline FLOAT
    toRadians(const FLOAT deg) { return (FLOAT) D3DXToRadian(deg); }

    inline FLOAT
    toDegrees(const FLOAT rad) { return (FLOAT) D3DXToDegree(rad); }

    inline FLOAT
    max2f(const FLOAT a, const FLOAT b) { return a > b ? a : b; }

    inline FLOAT
    max3f(const FLOAT a, const FLOAT b, const FLOAT c) { return max2f(a, max2f(b, c)); }

    inline FLOAT
    min2f(const FLOAT a, const FLOAT b) { return a < b ? a : b; }

    inline FLOAT
    min3f(const FLOAT a, const FLOAT b, const FLOAT c) { return min2f(a, min2f(b, c)); }

    inline FLOAT
    avg2f(const FLOAT a, const FLOAT b) { return (a + b) / 2.0f; }

    inline FLOAT
    avg3f(const FLOAT a, const FLOAT b, const FLOAT c) { return (a + b + c) / 3.0f; }

    
    /* ===============
       Class: Geometry
       =============== */
    class Geometry
    {
    public:
        
        Geometry(const vec3f& position);

        virtual
        ~Geometry() = 0;

        const vec3f&
        getPosition() logical_const;

        void
        setPosition(const vec3f& position);

    protected:

        vec3f m_position;

    };

    /* ==============
       Class: Frustum
       ============== */
    class Frustum: public Geometry
    {
    public:

        Frustum();

        ~Frustum();

        const plane&
        getPlane(size_t index) logical_const;

        void
        setPlane(const size_t index,
                 const plane& plane);

    private:

        plane m_planes[6];

    };


    /* ==========
       Class: Ray
       ========== */
    class Ray: public Geometry
    {
    public:

        Ray(const vec3f& position,
            const vec3f& direction);

        ~Ray();

        const vec3f&
        getDirection() logical_const;

        void
        setDirection(const vec3f& direction);

    private:

        vec3f m_direction;

    };

    /* =============
       Class: Sphere
       ============= */
    class Sphere: public Geometry
    {
    public:

        Sphere(const vec3f& position,
               const real32 radius);

        ~Sphere();

        const real32
        getRadius() logical_const;

        void
        setRadius(const real32 radius);

    private:

        real32 m_radius;

    };

    /* ===========
       Class: Cube
       =========== */
    class Cube: public Geometry
    {
    public:

        Cube(const vec3f& position,
             const vec3f& dimensions);

        ~Cube();

        const vec3f&
        getDimensions() logical_const;

        void
        setDimensions(const vec3f& dimensions);

    private:

        vec3f m_dimensions;

    };

    /* ============
       Class: Plane
       ============ */
    class GeoPlane: public Geometry
    {
    public:

        GeoPlane(const vec3f& position,
                 const vec2f& dimensions);

        ~GeoPlane();

        const vec2f&
        getDimensions() logical_const;

        void
        setDimensions(const vec2f& dimensions);

    private:

        vec2f m_dimensions;

    };
}