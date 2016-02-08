/* ----------------------------------------------------
   Author:           Alex Koukoulas
   Date:             8/2/2016
   File name:        basemanager.cpp

   File description: Implementation of the BaseManager
   class declared in basemanager.h
   ---------------------------------------------------- */

#include "basemanager.h"
#include "../util/physics.h"
#include "../game/scene.h"
#include "../game/tilemap.h"
#include "../game/eturret.h"
#include "../game/pathfinding.h"
#include "../game/command.h"
#include "../handlers/inputhandler.h"
#include "../util/logging.h"

BaseManager::BaseManager(Scene* scene, 
                         const Tilemap* levelTilemap, 
                         const Camera* camera):

                        m_sceneRef(scene),
                        m_tilemapRef(levelTilemap),
                        m_cameraRef(camera)
{   
    // Preload turret models
    Entity::bufferModel({"turret01_top", "turret01_base"});
    Entity::bufferModel({"turret02_top", "turret02_base"});
    Entity::bufferModel({"turret03_top", "turret03_base"});
}

BaseManager::~BaseManager()
{
}

void
BaseManager::update()
{
    if (InputHandler::get()->isTapped(InputHandler::KEY_Q))
    {
        Tile* selTile = physics::getHighlightedTile(m_cameraRef, m_tilemapRef);

        if (selTile)
        {
            selTile->t_flags |= TILE_FLAG_SOLID;

            if (pathfinding::findPath(m_tilemapRef, 
                                      m_tilemapRef->getTile(5, 0),
                                      m_tilemapRef->getTile(5, 10),
                                      nullptr,
                                      nullptr))
            {
                new ETurret("turret01",
                            m_cameraRef, 
                            m_tilemapRef, 
                            m_sceneRef, 
                            math::getVec3f(selTile->t_position), 
                            0.03f,
                            50.0f, 
                            30);
            }
            else
            {
                selTile->t_flags ^= TILE_FLAG_SOLID;
            }
        }
    }
}