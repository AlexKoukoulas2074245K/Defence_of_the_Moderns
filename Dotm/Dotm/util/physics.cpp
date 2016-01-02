/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             2/1/2016
   File name:        physics.cpp

   File description: Implementation to the physics
   functions declared in physics.h
   ------------------------------------------------ */

#include "physics.h"
#include "../game/camera.h"
#include "../handlers/inputhandler.h"
#include "../window.h"

/* -------------
   External Vars
   ------------- */
extern Window* g_window;

/* ---------------
   Public Function
   --------------- */
bool
physics::isPicked(const Mesh* mesh, const Camera* camera)
{    
    // Move mouse coords in normalized screen coords (-1, 1)
    real32 pointX, pointY;
    pointX = ((2.0f * InputHandler::get().getMousePos().x) / (real32) g_window->getWidth()) - 1.0f;
    pointY = -(((2.0f * InputHandler::get().getMousePos().y) / (real32) g_window->getHeight()) - 1.0f);

    // Unproject mouse coords from projection
    pointX /= camera->calculateProjectionMatrix()._11;
    pointY /= camera->calculateProjectionMatrix()._22;

    // Get the inverse view matrix
    mat4x4 viewmat = camera->calculateViewMatrix();
    D3DXMatrixInverse(&viewmat, NULL, &viewmat);

    // Calculate ray direction in view
    vec3f direction, origin, rayOrigin, rayDirection;
    direction.x = (pointX * viewmat._11) + (pointY * viewmat._21) + viewmat._31;
    direction.y = (pointX * viewmat._12) + (pointY * viewmat._22) + viewmat._32;
    direction.z = (pointX * viewmat._13) + (pointY * viewmat._23) + viewmat._33;

    // Get origin of picking ray from the cam pos
    origin = camera->getPosition();

    // Get each object's world matrix
    mat4x4 world = mesh->getWorldMatrix();
    D3DXMatrixInverse(&world, NULL, &world);

    // Transform ray origin and direction from view to world
    D3DXVec3TransformCoord(&rayOrigin, &origin, &world);
    rayDirection = direction;

    // Normalize ray direction
    D3DXVec3Normalize(&rayDirection, &rayDirection);
    real32 maxDim = math::avg3f(mesh->getDimensions().x,
                                mesh->getDimensions().y,
                                mesh->getDimensions().z);

    real32 sphereRad = maxDim / 2.0f;
    
    return raySphereIntersect(rayOrigin, rayDirection, sphereRad);
}

bool
physics::raySphereIntersect(const vec3f& rayOrigin,
                            const vec3f& rayDirection,
                            const real32 sphereRad)
{    
    // Find coefficients
    real32 a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
    real32 b = ((rayDirection.x * rayOrigin.x)   + (rayDirection.y * rayOrigin.y)    + (rayDirection.z * rayOrigin.z)) * 2.0f;
    real32 c = ((rayOrigin.x * rayOrigin.x)      + (rayOrigin.y * rayOrigin.y)       + (rayOrigin.z * rayOrigin.z)) - (sphereRad * sphereRad);

    // Find the discriminant
    real32 discriminant = (b * b) - (4 * a * c);
    if (discriminant > 0.0f) return true;
    return false;
}