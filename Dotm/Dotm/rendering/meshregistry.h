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

void
mr_registerMesh(const stringID meshName, const Mesh* mesh);

bool 
mr_entryExists(const stringID meshName);

const Mesh*
mr_retrieveMesh(const stringID meshName);

void 
mr_deleteEntry(const stringID meshName);
