/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        camera.cpp
   
   File description: Implementation of the
   virtual camera class and its children 
   declared in camera.h
   ----------------------------------------------- */

#include "camera.h"
#include "../window.h"
#include "../handlers/inputhandler.h"
#include "../config/configparser.h"
#include "../util/logging.h"
#include <cmath>

/* ---------------
   Class Constants
   --------------- */
const vec3f  Camera::CAM_DEFAULT_FORWARD(0.0f, 0.0f, -1.0f);
const vec3f  Camera::CAM_DEFAULT_RIGHT(1.0f, 0.0f, 0.0f);
const vec3f  Camera::CAM_DEFAULT_UP(0.0f, 1.0f, 0.0f);
const real32 Camera::CAM_ZNEAR(0.01f);
const real32 Camera::CAM_ZFAR(1000.0f);


/* -------------
   External Vars
   ------------- */
extern Window* g_window;

/* =============
   Class: Camera
   ============= */

/* --------------
   Public Methods
   -------------- */
Camera::Camera():

    m_position(0.0f, 0.0f, 10.0f),
    m_forward(CAM_DEFAULT_FORWARD),
    m_up(CAM_DEFAULT_UP),
    m_right(CAM_DEFAULT_RIGHT),
    m_pitch(0.0f),
    m_yaw(0.0f),
    m_roll(0.0f)    
{
    float horDragSpace, verDragSpace;
    config::initConfigFile("camconfig");
    config::extractConfigFloat("camconfig", "fov", &m_fov);
    config::extractConfigFloat("camconfig", "hor_drag_area_perc", &horDragSpace);
    config::extractConfigFloat("camconfig", "ver_drag_area_perc", &verDragSpace);
    config::extractConfigFloat("camconfig", "x_drag_speed", &m_xDragSpeed);
    config::extractConfigFloat("camconfig", "y_drag_speed", &m_yDragSpeed);
    config::extractConfigFloat("camconfig", "min_zoom", &m_minZoom);
    config::extractConfigFloat("camconfig", "max_zoom", &m_maxZoom);
    config::extractConfigFloat("camconfig", "zoom_speed", &m_zoomSpeed);
    config::extractConfigFloat("camconfig", "move_speed", &m_moveSpeed);
    config::extractConfigFloat("camconfig", "look_speed", &m_lookSpeed);    

    m_leftDragArea   = g_window->getWidth() / horDragSpace;
    m_rightDragArea  = (real32) (g_window->getWidth() - m_leftDragArea);
    m_topDragArea    = g_window->getHeight() / verDragSpace;
    m_bottomDragArea = (real32) (g_window->getHeight() - m_topDragArea);
}

Camera::~Camera()
{

}

void
Camera::update()
{        
    // Manual Zoom
    int32 wheelDelta = InputHandler::get().getWheelDelta();
    if (wheelDelta > 0 && m_fov > m_minZoom) m_fov -= m_zoomSpeed;
    if (wheelDelta < 0 && m_fov < m_maxZoom) m_fov += m_zoomSpeed;      

    // Manual movement control
    if (InputHandler::get().isPressed(InputHandler::KEY_W)) moveCamera(DIR_FORWARD, m_moveSpeed);
    if (InputHandler::get().isPressed(InputHandler::KEY_S)) moveCamera(DIR_BACKWARD, m_moveSpeed);
    if (InputHandler::get().isPressed(InputHandler::KEY_A)) moveCamera(DIR_LEFT, m_moveSpeed);
    if (InputHandler::get().isPressed(InputHandler::KEY_D)) moveCamera(DIR_RIGHT, m_moveSpeed);

    // Manual look control
    if (InputHandler::get().isPressed(InputHandler::KEY_UP))    rotateCamera(DIR_UP, m_lookSpeed);
    if (InputHandler::get().isPressed(InputHandler::KEY_DOWN))  rotateCamera(DIR_DOWN, m_lookSpeed);
    if (InputHandler::get().isPressed(InputHandler::KEY_LEFT))  rotateCamera(DIR_LEFT, m_lookSpeed);
    if (InputHandler::get().isPressed(InputHandler::KEY_RIGHT)) rotateCamera(DIR_RIGHT, m_lookSpeed);

}

void
Camera::moveCamera(const direction dir,
                   const real32 amount)
{ 
    switch (dir)
    {
        case DIR_FORWARD:
        {
            m_position.x -= std::sinf(m_yaw)   * amount;
            m_position.y += std::sinf(m_pitch) * amount;
            m_position.z -= std::cosf(m_yaw)   * amount;
        }break;

        case DIR_BACKWARD:
        {
            m_position.x += std::sinf(m_yaw)   * amount;
            m_position.y -= std::sinf(m_pitch) * amount;
            m_position.z += std::cosf(m_yaw)   * amount;
        }break;        
        
        case DIR_UP:
        {            
            m_position.y += amount;
        }break;

        case DIR_DOWN:
        {            
            m_position.y -= amount;
        }break;

        case DIR_LEFT:
        {            
            m_position.x -= std::sinf(m_yaw + PI_FL / 2) * amount;            
            m_position.z -= std::cosf(m_yaw + PI_FL / 2) * amount;
        }break;

        case DIR_RIGHT:
        {
            m_position.x += std::sinf(m_yaw + PI_FL / 2) * amount;            
            m_position.z += std::cosf(m_yaw + PI_FL / 2) * amount;            
        }break;
    }
}

void
Camera::rotateCamera(const direction dir,
                     const real32 amount)
{
    switch (dir)
    {
        case DIR_FORWARD:  m_roll  += amount; break;
        case DIR_BACKWARD: m_roll  -= amount; break;
        case DIR_UP:       m_pitch += amount; break;
        case DIR_DOWN:     m_pitch -= amount; break;
        case DIR_LEFT:     m_yaw   += amount; break;
        case DIR_RIGHT:    m_yaw   -= amount; break;        
    }
}

void
Camera::panCamera(const direction dir,
                  const real32 amount)
{
    switch (dir)
    {
        case DIR_FORWARD:
        {
            m_position.x -= std::sinf(m_yaw) * amount;
            m_position.z -= std::cosf(m_yaw) * amount;
        }break;

        case DIR_BACKWARD:
        {
            m_position.x += std::sinf(m_yaw) * amount;
            m_position.z += std::cosf(m_yaw) * amount;
        }break;

        case DIR_UP:
        {
            m_position.y -= std::sinf(m_pitch) * amount;
        }break;

        case DIR_DOWN:
        {
            m_position.y += std::sinf(m_pitch) * amount;
        }break;

        case DIR_LEFT:
        {
            m_position.x -= std::sinf(m_yaw + PI_FL / 2) * amount;
            m_position.z -= std::cosf(m_yaw + PI_FL / 2) * amount;
        }break;

        case DIR_RIGHT:
        {
            m_position.x += std::sinf(m_yaw + PI_FL / 2) * amount;
            m_position.z += std::cosf(m_yaw + PI_FL / 2) * amount;
        }break;
    }
}

mat4x4
Camera::calculateViewMatrix() logical_const
{
    vec3f up(CAM_DEFAULT_UP);
    vec3f forward(CAM_DEFAULT_FORWARD);
    vec3f right(CAM_DEFAULT_RIGHT);

    mat4x4 yawMatrix;
    D3DXMatrixRotationAxis(&yawMatrix, &up, m_yaw);
    D3DXVec3TransformCoord(&forward, &forward, &yawMatrix);
    D3DXVec3TransformCoord(&right, &right, &yawMatrix);

    mat4x4 pitchMatrix;
    D3DXMatrixRotationAxis(&pitchMatrix, &right, m_pitch);
    D3DXVec3TransformCoord(&forward, &forward, &pitchMatrix);
    D3DXVec3TransformCoord(&up, &up, &pitchMatrix);

    mat4x4 rollMatrix;
    D3DXMatrixRotationAxis(&rollMatrix, &forward, m_roll);
    D3DXVec3TransformCoord(&right, &right, &rollMatrix);
    D3DXVec3TransformCoord(&up, &up, &rollMatrix);

    mat4x4 viewMatrix;
    D3DXMatrixIdentity(&viewMatrix);

    viewMatrix._11 = right.x;
    viewMatrix._21 = right.y;
    viewMatrix._31 = right.z;

    viewMatrix._12 = up.x;
    viewMatrix._22 = up.y;
    viewMatrix._32 = up.z;

    viewMatrix._13 = forward.x;
    viewMatrix._23 = forward.y;
    viewMatrix._33 = forward.z;
    
    viewMatrix._41 = -D3DXVec3Dot(&m_position, &right);
    viewMatrix._42 = -D3DXVec3Dot(&m_position, &up);
    viewMatrix._43 = -D3DXVec3Dot(&m_position, &forward);

    return viewMatrix;
}

mat4x4
Camera::calculateProjectionMatrix() logical_const
{
    mat4x4 matres;
    D3DXMatrixPerspectiveFovLH(&matres,
                               math::toRadians(m_fov),
                               g_window->getAspect(),
                               CAM_ZNEAR,
                               CAM_ZFAR);
    return matres;
}

const vec3f&
Camera::getPosition() logical_const
{
    return m_position;
}

/* ==========================
   Class: WorldViewCamera
   ========================== */

/* --------------
   Public Methods
   -------------- */
WorldViewCamera::WorldViewCamera():

    Camera()
{
    m_position.x = -2.77f;
    m_position.y =  20.15f;
    m_position.z = -39.53f;
    m_pitch      = -0.30f;
    m_yaw        =  15.79f;
    m_roll       =  0.0f;
}

WorldViewCamera::~WorldViewCamera()
{

}

void
WorldViewCamera::update()
{
    screenEdgeTest();
    Camera::update();    
}

/* --------------- 
   Private Methods
   --------------- */
void
WorldViewCamera::screenEdgeTest()
{
    vec2f mousePos = InputHandler::get().getMousePos();

    if (mousePos.x < m_leftDragArea)   panCamera(DIR_LEFT,     m_xDragSpeed);
    if (mousePos.x > m_rightDragArea)  panCamera(DIR_RIGHT,    m_xDragSpeed);
    if (mousePos.y < m_topDragArea)    panCamera(DIR_FORWARD,  m_yDragSpeed);
    if (mousePos.y > m_bottomDragArea) panCamera(DIR_BACKWARD, m_yDragSpeed);    
}

