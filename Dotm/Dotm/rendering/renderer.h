/* ------------------------------------------------
Author:           Alex Koukoulas
Date:             15/12/2015
File name:        renderer.h

File description: A singleton class in charge of
the core rendering of meshes and default
geometry, along with controlling the
pause for each frame (when vsync is enabled).
------------------------------------------------ */

#pragma once

#include "../dotmdef.h"
#include <memory>

class D3D11State;
class Renderer
{
public:

    static Renderer&
        get();

    ~Renderer();

private:

    Renderer();

    Renderer(const Renderer& rhs) = delete;

    Renderer&
        operator = (const Renderer& rhs) = delete;

private:

    std::shared_ptr<D3D11State> m_d3dState;

};