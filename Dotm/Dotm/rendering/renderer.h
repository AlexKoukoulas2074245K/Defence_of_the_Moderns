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
#include <map>

class  D3D11State;
class  Mesh;
class  Camera;
class  Font;
class  Scene;
class  Renderer
{
public:

    enum Primitive
    {
        CUBE, PLANE, SPHERE
    };

    enum DebugColor
    {
        YELLOW, CYAN, RED
    };

public:

    static Renderer*
    get();

    ~Renderer();

    void
    beginFrame();
    
    void
    endFrame();

    void
    renderScene(const Scene* scene);

    void
    renderPrimitive(const Primitive       primitive, 
                    const math::Geometry* geometry,
                    const DebugColor      debugColor,
                    const bool            wireframe);

    void
    renderString(const cstring str,
                 const real32  x,
                 const real32  y);
    
    void
    renderMesh(const Mesh* mesh,
               const int highlightOption = 0);

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
    Shader*       m_bilShader;
    const Camera* m_currentCam;    
    Font*         m_font;
    
    std::map<Primitive, Mesh*> m_primitiveModels;
    Shader::PSCBuffer*         m_currentLightBuffer;

};