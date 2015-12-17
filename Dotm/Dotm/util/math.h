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

typedef D3DXMATRIX  mat4x4;
typedef D3DXVECTOR3 vec3;
typedef D3DXVECTOR4 vec4;