/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        math.h

   File description: A header file containing all
   the necessary linking and file inclusion to use
   the specified math structures and functions
   ------------------------------------------------ */

#pragma once

// Necessary link to d3dx10
#pragma comment(lib, "d3dx10.lib")

// d3dx10 header inclusion
#include <d3dx10.h>

#define PI_FL ((float) D3DX_PI)

typedef D3DXMATRIX  mat4x4;
typedef D3DXVECTOR2 vec2f;
typedef D3DXVECTOR3 vec3f;
typedef D3DXVECTOR4 vec4f;

namespace math
{
    inline vec4f
    getVec4f(const vec3f& in) { return D3DXVECTOR4(in.x, in.y, in.z, 0.0f); }

    inline vec3f
    getVec3f(const vec4f& in) { return D3DXVECTOR3(in.x, in.y, in.z); }

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
}