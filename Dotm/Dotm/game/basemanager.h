/* ----------------------------------------------------
   Author:           Alex Koukoulas
   Date:             8/2/2016
   File name:        basemanager.h

   File description: The interface of the base
   manager, responsible for turret placement, upgrading
   and management
   ---------------------------------------------------- */

#pragma once

class Tilemap;
class Scene;
class Camera;
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

    Scene*         m_sceneRef;
    const Tilemap* m_tilemapRef;
    const Camera*  m_cameraRef;
};