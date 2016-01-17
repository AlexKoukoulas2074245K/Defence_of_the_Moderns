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
physics::isPicked(const Mesh*   mesh,
                  const Camera* camera)
{    
    // Move mouse coords in normalized screen coords (-1, 1)
    real32 pointX, pointY;
    pointX = ((2.0f * InputHandler::get()->getMousePos().x) / (real32) g_window->getWidth()) - 1.0f;
    pointY = -(((2.0f * InputHandler::get()->getMousePos().y) / (real32) g_window->getHeight()) - 1.0f);

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
    mat4x4 trans = mesh->getTranslationMatrix();
    D3DXMatrixInverse(&trans, NULL, &trans);

    // Transform ray origin and direction from view to world
    D3DXVec3TransformCoord(&rayOrigin, &origin, &trans);
    rayDirection = direction;

    // Normalize ray direction
    D3DXVec3Normalize(&rayDirection, &rayDirection);
    
    // Create final ray
    math::Ray ray(rayOrigin, rayDirection);

    return intersectionTest(PHYSICS_INTERSECTION_TYPE_RAYSPHERE,
                            &ray,
                            &mesh->getCollidableGeometry());
}

bool
physics::intersectionTest(const uint32          intersectionType,
                          const math::Geometry* first,
                          const math::Geometry* second)
{
    switch (intersectionType)
    {
        case PHYSICS_INTERSECTION_TYPE_SPHERESPHERE:
        {
            const math::Sphere* firstSphere  = dynamic_cast<const math::Sphere*>(first);
            const math::Sphere* secondSphere = dynamic_cast<const math::Sphere*>(second);
            
            vec3f distVec = firstSphere->getPosition() - secondSphere->getPosition();
            return D3DXVec3Length(&distVec) < firstSphere->getRadius() + secondSphere->getRadius();

        }break;

        case PHYSICS_INTERSECTION_TYPE_RAYSPHERE:
        {
            const math::Ray* ray       = dynamic_cast<const math::Ray*>(first);
            const math::Sphere* sphere = dynamic_cast<const math::Sphere*>(second);

            // Find coefficients
            real32 a = (ray->getDirection().x * ray->getDirection().x) +
                       (ray->getDirection().y * ray->getDirection().y) +
                       (ray->getDirection().z * ray->getDirection().z);

            real32 b = ((ray->getDirection().x * ray->getPosition().x) +
                        (ray->getDirection().y * ray->getPosition().y) +
                        (ray->getDirection().z * ray->getPosition().z)) * 2.0f;

            real32 c = ((ray->getPosition().x * ray->getPosition().x) +
                        (ray->getPosition().y * ray->getPosition().y) +
                        (ray->getPosition().z * ray->getPosition().z)) -
                        (sphere->getRadius() * sphere->getRadius());
            
            // Find the discriminant
            real32 discriminant = (b * b) - (4 * a * c);
            if (discriminant > 0.0f) return true;
            return false;

        }break;

        case PHYSICS_INTERSECTION_TYPE_FRUSTUMSPHERE:
        {
            const math::Frustum* frustum = dynamic_cast<const math::Frustum*>(first);
            const math::Sphere*  sphere  = dynamic_cast<const math::Sphere*>(second);
            
            for (size_t i = 0;
                        i < 6;
                      ++i)
            {
                if (D3DXPlaneDotCoord(&frustum->getPlane(i),
                                      &sphere->getPosition()) < -sphere->getRadius())
                {
                    return true;
                }
            }

            return false;

        }break;
    }

    return false;
}