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
#include "../game/eturret.h"
#include "../game/pathfinding.h"
#include "../game/command.h"
#include "../handlers/inputhandler.h"
#include "../util/logging.h"

/* ---------------
   Class Constants
   --------------- */
const real32 BaseManager::BM_HI_HIGHLIGHT_VAL = 1.0f;
const real32 BaseManager::BM_LO_HIGHLIGHT_VAL = 0.7f;
const real32 BaseManager::BM_HIGHLIGHT_INC_VAL = 0.006f;

/* --------------
   Public Methods
   -------------- */
BaseManager::BaseManager(Scene* scene, 
                         const Tilemap* levelTilemap, 
                         const Camera* camera):

                         m_sceneRef(scene),
                         m_tilemapRef(levelTilemap),
                         m_cameraRef(camera),
                         m_selLerpVal(BM_LO_HIGHLIGHT_VAL),
                         m_selValFlow(1),
                         m_state(BMState::HIGHLIGHTING)
{   
    // Preload turret models
    Entity::bufferModel({"turret01_top", "turret01_base"});
    Entity::bufferModel({"turret02_top", "turret02_base"});
    Entity::bufferModel({"turret03_top", "turret03_base"});

    m_tileEntity = new Entity("sel_tile",
                           {"sample_plane"},
                           m_cameraRef, 
                           m_tilemapRef, 
                           m_sceneRef, 
                           Entity::ENTITY_PROPERTY_STATIC,
                           vec3f(0.0f, 0.0f, 0.0f),
                           "sel_tile");

    real32 scaleFactor = m_tilemapRef->getTileSize() / 
                         m_tileEntity->getBody()->calculateDimensions().x;

    m_tileEntity->getBody()->scale = {scaleFactor, 0.0f, scaleFactor};
    m_tileEntity->setInvisible(true);
    m_tileEntity->getBody()->setNoLighting(true);   
    m_tileEntity->getBody()->setSpecialPCBuffer(&m_customPCBuffer);
}

BaseManager::~BaseManager()
{    
}

void
BaseManager::update()
{
    switch (m_state)
    {
        case BMState::HIGHLIGHTING: 
        {
            m_selLerpVal = BM_HI_HIGHLIGHT_VAL;
            
            
            // Grab Selected Tile if any
            m_tileEntity->setInvisible(true);
            m_targetTile = physics::getHighlightedTile(m_cameraRef, m_tilemapRef);

            if (m_targetTile)
            {
                m_tileEntity->getBody()->position = math::getVec3f(m_targetTile->t_position);
                m_tileEntity->getBody()->position.y += 0.2f; // avoid z-fighting

                if (!m_targetTile->isSolid() && !isEnemyResidentInTile(m_targetTile))
                {
                    m_tileEntity->setInvisible(false);
                }
            }

            // Change State on Click
            if (InputHandler::get()->isTapped(InputHandler::BUTTON_LEFT))                
            {
                m_state = BMState::SELECTED;
            }

        } break;

        case BMState::SELECTED:
        {
            // Lerp selector color
            m_selLerpVal += m_selValFlow * BM_HIGHLIGHT_INC_VAL;
            if (m_selLerpVal > BM_HI_HIGHLIGHT_VAL ||
                m_selLerpVal < BM_LO_HIGHLIGHT_VAL) m_selValFlow *= -1;
            m_customPCBuffer.pcb_directionalLights[0]._padding = m_selLerpVal;

            if (InputHandler::get()->isTapped(InputHandler::BUTTON_LEFT))                
            {
                m_state = BMState::HIGHLIGHTING;
            }

            if (InputHandler::get()->isTapped(InputHandler::KEY_Q) && m_targetTile)
            {
                // Calculate A* to see if valid tile
                m_targetTile->t_flags |= TILE_FLAG_SOLID;

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
                        math::getVec3f(m_targetTile->t_position),
                        0.03f,
                        50.0f,
                        30);

                    m_state = HIGHLIGHTING;
                }
                else
                {
                    m_targetTile->t_flags ^= TILE_FLAG_SOLID;
                }
            }
        } break;
    }    
}


/* --------------- 
   Private Methods
   --------------- */
bool
BaseManager::isEnemyResidentInTile(const Tile* tile)
{
    for (auto iter = m_sceneRef->getEnemies().begin();
              iter != m_sceneRef->getEnemies().end();
            ++iter)
    {
        if (!(*iter)->isAlive()) continue;
        if (m_tilemapRef->getTile((*iter)->getBody()->position) == tile)
        {
            return true;
        }
    }   

    return false;
}