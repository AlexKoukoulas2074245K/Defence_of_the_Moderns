/* -------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        meshregistry.h

   File description: A collection of functions
   controlling a mesh registry mechanism 
   which avoids mesh duplicates and is based
   on keys of type stringID
   ------------------------------------------- */
#pragma once

#include "../util/strings.h"

class Mesh;
class Texture;
namespace resource
{
    // Mesh interface
    void
    registerMesh(const stringID meshName, Mesh* mesh);

    bool 
    meshExists(const stringID meshName);

    Mesh*
    retrieveMesh(const stringID meshID);
    
    Mesh*
    retrieveMesh(cstring meshName);

    void 
    deleteMesh(const stringID meshName);
}
