/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        meshregistry.cpp

   File description: Implementation of the 
   functions controlling the mesh registry
   declared in meshregistry.h
   ------------------------------------------ */

#include "meshregistry.h"
#include <unordered_map>

/* -------------
   Internal Vars
   ------------- */
static std::unordered_map<stringID, const Mesh*> registry;

/* ----------------
   Public Functions
   ---------------- */
void
mr_registerMesh(const stringID meshName, const Mesh* mesh)
{
    registry[meshName] = mesh;
}

bool 
mr_entryExists(const stringID meshName)
{
    return registry.count(meshName) != 0;
}

const Mesh*
mr_retrieveMesh(const stringID meshName)
{
    if (!mr_entryExists(meshName)) return nullptr;
    return registry[meshName];
}

void
mr_deleteEntry(const stringID meshName)
{
    registry.erase(meshName);
}