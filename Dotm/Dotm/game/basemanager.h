/* ----------------------------------------------------
   Author:           Alex Koukoulas
   Date:             8/2/2016
   File name:        basemanager.h

   File description: The interface of the base
   manager, responsible for turret placement, upgrading
   and management
   ---------------------------------------------------- */

#pragma once
#include "../rendering/shader.h"
#include "../game/tilemap.h"

class  Scene;
class  Camera;
class  Entity;
struct Shader::PSCBuffer;
class BaseManager
{
public:

    BaseManager(Scene* scene, 
                const Tilemap* levelTilemap, 
                const Camera* camera);

    ~BaseManager();

    BaseManager(const BaseManager& rhs) = delete;

    BaseManager&
    operator = (const BaseManager& rhs) = delete;

    void
    update();

private:

    bool
    isEnemyResidentInTile(const Tile* tile);

private:

    enum BMState
    {
        HIGHLIGHTING, SELECTED
    };

private:

    static const real32 BM_HI_HIGHLIGHT_VAL;
    static const real32 BM_LO_HIGHLIGHT_VAL;
    static const real32 BM_HIGHLIGHT_INC_VAL;

private:

    BMState           m_state;
    real32            m_selLerpVal;
    int32             m_selValFlow;
    Scene*            m_sceneRef;
    Entity*           m_tileEntity;
    Tile*             m_targetTile;
    const Tilemap*    m_tilemapRef;
    const Camera*     m_cameraRef;
    Shader::PSCBuffer m_customPCBuffer;

};