/* ---------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        dotmdef.h

   File description: Common typedefs
   used throughout the project
   --------------------------------- */

#pragma once

#define bitwise_const const
#define logical_const const

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long ulong64;
typedef unsigned long long ullong64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long64;
typedef signed long long llong64;

typedef float real32;
typedef float real64;

enum direction
{
    DIR_FORWARD, DIR_BACKWARD,
    DIR_LEFT,    DIR_RIGHT,
    DIR_UP,      DIR_DOWN
};
