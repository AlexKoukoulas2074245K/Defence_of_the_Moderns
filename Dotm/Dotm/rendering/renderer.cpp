/* ---------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        renderer.cpp

   File description: The implementation of the
   renderer class declared in renderer.h.
   --------------------------------------------- */

#include "renderer.h"
#include "shader.h"
#include "resregistry.h"
#include "mesh.h"
#include "../window.h"

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
    if (m_d3dState) delete m_d3dState;
    if (m_stdShader) delete m_stdShader;
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
Renderer::renderMesh(const stringID meshName)
{
    const Mesh* mesh = resource::retrieveMesh(meshName);

    uint32 stride = sizeof(Mesh::Vertex);
    uint32 offset = 0U;

    D3DXMATRIX projMatrix;
    D3DXMatrixPerspectiveFovLH(&projMatrix, (real32)D3DXToRadian(45.0f), g_window->getAspect(), 0.001f, 100.0f);
    
    D3DXVECTOR3 eye(0.0f, 0.0f, 10.0f);
    D3DXVECTOR3 tar(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 upv(0.0f, 1.0f, 0.0f);
    D3DXMATRIX viewMatrix;
    D3DXMatrixLookAtLH(&viewMatrix, &eye, &tar, &upv);

    D3DXMATRIX worldMat = mesh->getWorldMatrix();
    D3DXMATRIX finalMat = worldMat * viewMatrix * projMatrix;
    D3DXMATRIX rotMat   = mesh->getRotationMatrix();

    Shader::VSCBuffer vcbuffer = {};
    vcbuffer.eyePosition    = D3DXVECTOR4();
    vcbuffer.mvpMatrix      = finalMat;
    vcbuffer.rotationMatrix = rotMat;
    vcbuffer.worldMatrix    = worldMat;

    Shader::PSCBuffer pcbuffer = {};
   
    m_d3dState->m_devcon->UpdateSubresource(m_stdShader->getVSCBuffer().Get(), NULL, NULL, &vcbuffer, NULL, NULL);
    m_d3dState->m_devcon->UpdateSubresource(m_stdShader->getPSCBuffer().Get(), NULL, NULL, &pcbuffer, NULL, NULL);

    m_d3dState->m_devcon->IASetVertexBuffers(0, 1, mesh->getVertexBuffer().GetAddressOf(), &stride, &offset);
    m_d3dState->m_devcon->IASetIndexBuffer(mesh->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0U);
    m_d3dState->m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    m_d3dState->m_devcon->VSSetShader(m_stdShader->getVertexShader().Get(), NULL, NULL);
    m_d3dState->m_devcon->VSSetConstantBuffers(0, 1, m_stdShader->getVSCBuffer().GetAddressOf());

    m_d3dState->m_devcon->PSSetShader(m_stdShader->getPixelShader().Get(), NULL, NULL);
    m_d3dState->m_devcon->PSSetConstantBuffers(0, 1, m_stdShader->getPSCBuffer().GetAddressOf());
    m_d3dState->m_devcon->PSSetShaderResources(0, 1, mesh->getTexture().GetAddressOf());
            
    m_d3dState->m_devcon->DrawIndexed(mesh->getIndexCount(), 0, 0);    
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
    m_stdShader(new Shader("std"))
{
  
}