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

namespace resource
{
    void
    registerMesh(const stringID meshName, const Mesh* mesh);

    bool 
    meshExists(const stringID meshName);

    const Mesh*
    retrieveMesh(const stringID meshName);

    void 
    deleteMesh(const stringID meshName);
}
