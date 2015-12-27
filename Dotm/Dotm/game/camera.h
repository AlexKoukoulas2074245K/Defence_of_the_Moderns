/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        camera.h
   
   File description: The interface of a class 
   representing a virtual camera in the game
   world along with its children.
   ----------------------------------------------- */

#pragma once

#include "../util/math.h"
#include "../dotmdef.h"

/* =============
   Class: Camera
   ============= */
class Camera
{
public:

    Camera();

    virtual
    ~Camera();

    virtual void
    update();

    void
    moveCamera(const direction dir,
               const real32 amount);

    void
    rotateCamera(const direction dir,
                 const real32 amount);

    void
    panCamera(const direction dir,
              const real32 amount);

    mat4x4
    calculateViewMatrix() logical_const;

    mat4x4
    calculateProjectionMatrix() logical_const;

protected:
    
    static const vec3f  CAM_DEFAULT_FORWARD;
    static const vec3f  CAM_DEFAULT_RIGHT;
    static const vec3f  CAM_DEFAULT_UP;

    static const real32 CAM_ZNEAR;
    static const real32 CAM_ZFAR;       

protected:
    
    vec3f  m_position;
    vec3f  m_forward;
    vec3f  m_up;
    vec3f  m_right;
    real32 m_pitch;
    real32 m_yaw;
    real32 m_roll;
    real32 m_fov;

    real32 m_leftDragArea;
    real32 m_rightDragArea;
    real32 m_topDragArea;
    real32 m_bottomDragArea;
    real32 m_xDragSpeed;
    real32 m_yDragSpeed;     
    
    real32 m_minZoom;
    real32 m_maxZoom;
    real32 m_zoomSpeed;

    real32 m_moveSpeed;
    real32 m_lookSpeed;
};

/* ==========================
   Class: WorldViewCamera
   ========================== */

class WorldViewCamera: public Camera
{
public:

    WorldViewCamera();
    
    ~WorldViewCamera();

    void
    update();

private:

    void
    screenEdgeTest();

  

};