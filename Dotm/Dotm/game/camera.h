/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        camera.h
   
   File description: The interface of a class 
   representing a virtual camera in the game
   world. All calculations are made with degrees
   and only during the matrix calculations they
   are transformed to radians.
   ----------------------------------------------- */

#pragma once

#include "../util/math.h"
#include "../dotmdef.h"

class Camera
{
public:

    Camera();

    ~Camera();

    Camera(const Camera& rhs);

    Camera& 
    operator = (const Camera& rhs);

    void
    update();

    mat4x4
    calculateViewMatrix() logical_const;

    mat4x4
    calculateProjectionMatrix() logical_const;

    const vec3f&
    getPosition() logical_const;

    const vec3f&
    getForward() logical_const;

    const vec3f&
    getRight() logical_const;

    const vec3f&
    getUp() logical_const;

    const real32&
    getPitch() logical_const;

    const real32&
    getYaw() logical_const;

    const real32&
    getRoll() logical_const;

    const real32&
    getFOV() logical_const;

    void
    setPosition(const vec3f& position);

    void
    setForward(const vec3f& forward);

    void
    setRight(const vec3f& right);

    void
    setUp(const vec3f& up);

    void
    setPitch(const real32 pitch);

    void
    setYaw(const real32 yaw);
    
    void
    setRoll(const real32 roll);

    void
    setFOV(const real32 fov);

private:

    void
    init(const Camera& rhs);

private:

    static const real32 CAM_DEFAULT_FOV;
    static const vec3f  CAM_DEFAULT_FORWARD;
    static const vec3f  CAM_DEFAULT_RIGHT;
    static const vec3f  CAM_DEFAULT_UP;

    static const real32 CAM_ZNEAR;
    static const real32 CAM_ZFAR;
    
    static const real32 CAM_MAX_ZOOM;
    static const real32 CAM_MIN_ZOOM;

private:
    
    vec3f  m_position;
    vec3f  m_forward;
    vec3f  m_up;
    vec3f  m_right;
    real32 m_pitch;
    real32 m_yaw;
    real32 m_roll;
    real32 m_fov;

};