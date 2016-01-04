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
#include "shader.h"
#include "../dotmdef.h"
#include "../util/strings.h"
#include "../util/math.h"
#include <vector>

class  D3D11State;
class  Mesh;
class  Camera;
class  Font;
class  Renderer
{
public:

    static const uint32 RENDERER_PRIMITIVE_PLANE  = 0;
    static const uint32 RENDERER_PRIMITIVE_CUBE   = 1;
    static const uint32 RENDERER_PRIMITIVE_SPHERE = 2;

public:

    static Renderer*
    get();

    ~Renderer();

    void
    beginFrame();
    
    void
    endFrame();

    void
    renderScene();

    void
    renderPrimitive(const uint32 primitive, 
                    const math::Geometry* geometry,
                    bool wireframe);

    void
    renderString(const cstring str,
                 const real32 x,
                 const real32 y);
    
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

    bool
    testVisible(const Mesh* mesh);

private:

    D3D11State*   m_d3dState;
    Shader*       m_stdShader;
    Shader*       m_hudShader;
    const Camera* m_currentCam;
    Mesh*         m_primitiveModels[3];
    Font*         m_font;
    
    Shader::PSCBuffer* m_currentLightBuffer;

};