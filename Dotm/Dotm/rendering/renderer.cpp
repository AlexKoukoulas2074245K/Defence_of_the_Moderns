/* ---------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        renderer.cpp

   File description: The implementation of the
   renderer class declared in renderer.h.
   --------------------------------------------- */

#include "renderer.h"
#include "resregistry.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "font.h"
#include "../game/camera.h"
#include "../window.h"
#include "../util/logging.h"

/* -------------
   External Vars
   ------------- */
extern Window* g_window;

/* --------------
   Public Methods
   -------------- */
Renderer& Renderer::get()
{
    static Renderer instance;
    return instance;
}

Renderer::~Renderer()
{
    if (m_d3dState)  delete m_d3dState;
    if (m_stdShader) delete m_stdShader;
    if (m_hudShader) delete m_hudShader;
    if (m_font)      delete m_font;
    
    for (size_t i = 0; i < ARRAYSIZE(m_primitiveModels); ++i)
    {
        if (m_primitiveModels[i]) delete m_primitiveModels[i];
    }

}

void
Renderer::beginFrame()
{
    FLOAT clearColor[4] = {1.0f, 0.0f, 0.6f, 1.0f};
    m_d3dState->m_devcon->ClearRenderTargetView(m_d3dState->m_backBuffer.Get(),
                                                clearColor);
    
    m_d3dState->m_devcon->ClearDepthStencilView(m_d3dState->m_depthStencilView.Get(),
                                                D3D11_CLEAR_DEPTH,
                                                1.0f,
                                                0);
}

void
Renderer::endFrame()
{    
    uint32 syncInterval = m_d3dState->m_vsync ? 1 : 0;
    m_d3dState->m_swapChain->Present(syncInterval, 0);    
}

void
Renderer::renderPrimitive(const uint32 primitive, 
                          const vec3f& position,
                          const vec3f& dimensions,
                          const bool   wireframe)
{
    if (wireframe) m_d3dState->m_devcon->RSSetState(m_d3dState->m_wireFrameRastState.Get());
    
    Mesh* mesh = nullptr;
    
    switch (primitive)
    {
        case RENDERER_PRIMITIVE_CUBE:
        {
            mesh = m_primitiveModels[RENDERER_PRIMITIVE_CUBE];            
        }break;

        case RENDERER_PRIMITIVE_PLANE:
        {
            mesh = m_primitiveModels[RENDERER_PRIMITIVE_PLANE];            
        }break;

        case RENDERER_PRIMITIVE_SPHERE:
        {
            mesh = m_primitiveModels[RENDERER_PRIMITIVE_SPHERE];             
        }break;
    }
    
    if (mesh)
    {
        real32 maxMeshDim = math::avg3f(dimensions.x,
                                        dimensions.y,
                                        dimensions.z);
        
        mesh->scaleX *= maxMeshDim / mesh->getDimensions().x;
        mesh->scaleY *= primitive == RENDERER_PRIMITIVE_PLANE ? 0 :
                        maxMeshDim / mesh->getDimensions().y;
        mesh->scaleZ *= maxMeshDim / mesh->getDimensions().z;
        
        mesh->x = position.x;
        mesh->y = position.y;
        mesh->z = position.z;
        
        renderMesh(mesh);
    }

    if (wireframe) m_d3dState->m_devcon->RSSetState(m_d3dState->m_solidRastState.Get());
}

void
Renderer::renderString(const cstring str,
                       const real32 x,
                       const real32 y)
{
    real32 xCounter = x;
    for (size_t i = 0;
         i < strlen(str);
         ++i, xCounter += m_font->getSize() / 1.5f)
    {
        uint8 currChar = str[i];
        if (currChar == ' ') continue;

        std::shared_ptr<Mesh> currGlyph = m_font->getGlyph(currChar);

        currGlyph->x = xCounter;
        currGlyph->y = y;
        renderMesh(currGlyph.get());        
    }
}

void
Renderer::renderMesh(const cstring meshName)
{   
    const Mesh* mesh = resource::retrieveMesh(meshName);
    if (!mesh)
    {
        logstring("Could not find mesh: ");
        logline(meshName);        
    }
    renderMesh(resource::retrieveMesh(meshName));
}

void
Renderer::renderMesh(const Mesh* mesh)
{
    if (!mesh) return;
    if (!m_currentCam)
    {
        logline("Camera has not been set");
        return;
    }

    uint32 stride = sizeof(Mesh::Vertex);
    uint32 offset = 0U;

    mat4x4 projMatrix = m_currentCam->calculateProjectionMatrix();
    mat4x4 viewMatrix = m_currentCam->calculateViewMatrix();    

    mat4x4 worldMat = mesh->getWorldMatrix();
    mat4x4 finalMat = worldMat * viewMatrix * projMatrix;
    mat4x4 rotMat   = mesh->getRotationMatrix();

    Shader::VSCBuffer vcbuffer = {};
    vcbuffer.mvpMatrix         = finalMat;
    vcbuffer.rotationMatrix    = rotMat;
    vcbuffer.worldMatrix       = worldMat;
    vcbuffer.eyePosition       = math::getVec4f(m_currentCam->getPosition());

    Shader::PSCBuffer pcbuffer = {};
    
    Shader* currentShader = mesh->isHUDElement() ? m_hudShader : m_stdShader;
    m_d3dState->m_devcon->UpdateSubresource(currentShader->getVSCBuffer().Get(), NULL, NULL, &vcbuffer, NULL, NULL);
    m_d3dState->m_devcon->UpdateSubresource(currentShader->getPSCBuffer().Get(), NULL, NULL, &pcbuffer, NULL, NULL);

    m_d3dState->m_devcon->IASetVertexBuffers(0, 1, mesh->getVertexBuffer().GetAddressOf(), &stride, &offset);
    m_d3dState->m_devcon->IASetIndexBuffer(mesh->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0U);
    m_d3dState->m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    m_d3dState->m_devcon->VSSetShader(currentShader->getVertexShader().Get(), NULL, NULL);
    m_d3dState->m_devcon->VSSetConstantBuffers(0, 1, currentShader->getVSCBuffer().GetAddressOf());

    m_d3dState->m_devcon->PSSetShader(currentShader->getPixelShader().Get(), NULL, NULL);
    m_d3dState->m_devcon->PSSetConstantBuffers(0, 1, currentShader->getPSCBuffer().GetAddressOf());
    m_d3dState->m_devcon->PSSetShaderResources(0, 1, mesh->getTexture()->getTexturePointer().GetAddressOf());
    
    if  (mesh->isHUDElement()) m_d3dState->m_devcon->OMSetDepthStencilState(m_d3dState->m_disabledDepth.Get(), 1);
    if (!mesh->isHUDElement()) m_d3dState->m_devcon->OMSetDepthStencilState(m_d3dState->m_enabledDepth.Get(), 1);
    
    m_d3dState->m_devcon->DrawIndexed(mesh->getIndexCount(), 0, 0);    
}

void
Renderer::setCamera(const Camera* camera)
{
    m_currentCam = camera;
}

comptr<ID3D11Device>
Renderer::getDeviceHandle() bitwise_const
{
    return m_d3dState->m_device;
}

comptr<ID3D11DeviceContext>
Renderer::getDevconHandle() bitwise_const
{
    return m_d3dState->m_devcon;
}

/* ---------------
   Private Methods
   --------------- */
Renderer::Renderer():

    m_d3dState(new D3D11State()),        
    m_stdShader(new Shader("std")),
    m_hudShader(new Shader("hud")),
    m_font(new Font("font_1", 0.1f))
{
    m_primitiveModels[RENDERER_PRIMITIVE_CUBE]   = new Mesh("sample_cube", Mesh::MESH_TYPE_NORMAL);
    m_primitiveModels[RENDERER_PRIMITIVE_PLANE]  = new Mesh("sample_plane", Mesh::MESH_TYPE_NORMAL);    
    m_primitiveModels[RENDERER_PRIMITIVE_SPHERE] = new Mesh("sample_sphere", Mesh::MESH_TYPE_NORMAL);

    m_primitiveModels[RENDERER_PRIMITIVE_CUBE]->loadNewTexture("debug");
    m_primitiveModels[RENDERER_PRIMITIVE_PLANE]->loadNewTexture("debug");
    m_primitiveModels[RENDERER_PRIMITIVE_SPHERE]->loadNewTexture("debug");
}

