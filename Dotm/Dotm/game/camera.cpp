/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        camera.cpp
   
   File description: Implementation of the
   virtual camera class declared in camera.h
   ----------------------------------------------- */

#include "camera.h"
#include "../window.h"
#include "../handlers/inputhandler.h"
#include "../util/logging.h"
#include <string>

/* ---------------
   Class Constants
   --------------- */
const vec3f  Camera::CAM_DEFAULT_FORWARD(0.0f, 0.0f, -1.0f);
const vec3f  Camera::CAM_DEFAULT_RIGHT(1.0f, 0.0f, 0.0f);
const vec3f  Camera::CAM_DEFAULT_UP(0.0f, 1.0f, 0.0f);
const real32 Camera::CAM_DEFAULT_FOV(45.0f);
const real32 Camera::CAM_ZNEAR(0.01f);
const real32 Camera::CAM_ZFAR(1000.0f);
const real32 Camera::CAM_MAX_ZOOM(10);
const real32 Camera::CAM_MIN_ZOOM(1000.0f);

/* -------------
   External Vars
   ------------- */
extern Window* g_window;


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
    m_roll(0.0f),
    m_fov(CAM_DEFAULT_FOV)
{

}

Camera::~Camera()
{

}

Camera::Camera(const Camera& rhs)
{
    init(rhs);
}

Camera&
Camera::operator = (const Camera& rhs)
{
    init(rhs);
    return *this;
}

void
Camera::update()
{
    int32 wheelDelta = InputHandler::get().getWheelDelta();
    if (wheelDelta > 0 && m_fov > CAM_MAX_ZOOM) m_fov -= 3.0f;
    if (wheelDelta < 0 && m_fov < CAM_MIN_ZOOM) m_fov += 3.0f;
    
}

mat4x4
Camera::calculateViewMatrix() logical_const
{
    vec3f      up(CAM_DEFAULT_UP);
    vec3f forward(CAM_DEFAULT_FORWARD);
    vec3f   right(CAM_DEFAULT_RIGHT);

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
                               toRadians(m_fov),
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

const vec3f&
Camera::getForward() logical_const
{
    return m_forward;
}

const vec3f&
Camera::getRight() logical_const
{
    return m_right;
}

const vec3f&
Camera::getUp() logical_const
{
    return m_up;
}

const real32&
Camera::getPitch() logical_const
{
    return m_pitch;
}

const real32&
Camera::getYaw() logical_const
{
    return m_yaw;
}

const real32&
Camera::getRoll() logical_const
{
    return m_roll;
}

const real32&
Camera::getFOV() logical_const
{
    return m_fov;
}

void
Camera::setPosition(const vec3f& position)
{
    m_position = position;
}

void
Camera::setForward(const vec3f& forward)
{
    m_forward = forward;
}

void
Camera::setRight(const vec3f& right)
{
    m_right = right;
}

void
Camera::setUp(const vec3f& up)
{
    m_up = up;
}

void
Camera::setPitch(const real32 pitch)
{
    m_pitch = pitch;
}

void
Camera::setYaw(const real32 yaw)
{
    m_yaw = yaw;
}

void
Camera::setRoll(const real32 roll)
{
    m_roll = roll;
}

void
Camera::setFOV(const real32 fov)
{
    m_fov = fov;
}

/* --------------- 
   Private Methods
   --------------- */
void
Camera::init(const Camera& rhs)
{
    m_position = rhs.getPosition();
    m_forward = rhs.getForward();
    m_right = rhs.getRight();
    m_up = rhs.getUp();
    m_pitch = rhs.getPitch();
    m_yaw = rhs.getYaw();
    m_roll = rhs.getRoll();
    m_fov = rhs.getFOV();
}