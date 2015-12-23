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


#include "d3d11state.h"
#include "../dotmdef.h"
#include "../util/strings.h"

class D3D11State;
class Mesh;
class Shader;
class Camera;
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
    renderMesh(const cstring meshName);

    void
    renderMesh(const Mesh* mesh);

    void
    setCamera(const Camera* camera);

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
    const Camera* m_currentCam;
};