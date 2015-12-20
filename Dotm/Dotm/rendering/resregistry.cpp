/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        meshregistry.cpp

   File description: Implementation of the 
   functions controlling the mesh registry
   declared in meshregistry.h
   ------------------------------------------ */

#include "resregistry.h"
#include <unordered_map>

/* -------------
   Internal Vars
   ------------- */
static std::unordered_map<stringID, const Mesh*> registry;

/* ----------------
   Public Functions
   ---------------- */
void
resource::registerMesh(const stringID meshName, const Mesh* mesh)
{
    registry[meshName] = mesh;
}

bool 
resource::meshExists(const stringID meshName)
{
    return registry.count(meshName) != 0;
}

const Mesh*
resource::retrieveMesh(const stringID meshName)
{
    if (!meshExists(meshName)) return nullptr;
    return registry[meshName];
}

void
resource::deleteMesh(const stringID meshName)
{
    registry.erase(meshName);
}