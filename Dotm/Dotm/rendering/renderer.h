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
#include "../util/strings.h"
#include "d3d11state.h"

class D3D11State;
class Shader;
class Renderer
{
public:

    static Renderer&
    get();

    ~Renderer();

    void
    beginFrame();
    
    void
    endFrame();

    void
    renderMesh(const stringID meshName);

    comptr<ID3D11Device>
    getDeviceHandle() bitwise_const;

    comptr<ID3D11DeviceContext>
    getDevconHandle() bitwise_const;

private:

    Renderer();

    Renderer(const Renderer& rhs) = delete;

    Renderer& 
    operator = (const Renderer& rhs) = delete;

private:

    D3D11State* m_d3dState;
    Shader* m_stdShader;

};