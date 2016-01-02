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
static std::unordered_map<stringID, Mesh*> meshRegistry;

/* ----------------
   Public Functions
   ---------------- */
void
resource::registerMesh(const stringID meshName, Mesh* mesh)
{
    meshRegistry[meshName] = mesh;
}

bool 
resource::meshExists(const stringID meshName)
{
    return meshRegistry.count(meshName) != 0;
}

Mesh*
resource::retrieveMesh(const stringID meshID)
{
    if (!meshExists(meshID)) return nullptr;
    return meshRegistry[meshID];
}

Mesh*
resource::retrieveMesh(cstring meshName)
{    
    for (auto citer = meshRegistry.cbegin();
        citer != meshRegistry.cend();
        ++citer)
    {
        if (!strcmp(meshName, retrieveString(citer->first))) 
            return citer->second;
    }
    return nullptr;
}

void
resource::deleteMesh(const stringID meshName)
{
    meshRegistry.erase(meshName);
}