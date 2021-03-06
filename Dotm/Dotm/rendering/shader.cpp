/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        shader.cpp

   File description: An implementation of the
   shader class declared in shader.h
   ------------------------------------------------ */

#include "shader.h"
#include "renderer.h"
#include "../util/stringutils.h"
#include <string>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

/* --------------
   Public Methods
   -------------- */
Shader::Shader(cstring shaderName)
{
               createVertexPixelShaders(shaderName);
               createConstantBuffers();
               createShaderLayout();
}

Shader::~Shader()
{

}

comptr<ID3D11InputLayout>
Shader::getShaderLayout() bitwise_const
{
    return m_shaderInputLayout;
}

comptr<ID3D11VertexShader>
Shader::getVertexShader() bitwise_const
{
    return m_vertexShader;
}

comptr<ID3D11PixelShader>
Shader::getPixelShader() bitwise_const
{
    return m_pixelShader;
}

comptr<ID3D11Buffer>
Shader::getVSCBuffer() bitwise_const
{
    return m_vertexShaderCBuffer;
}

comptr<ID3D11Buffer>
Shader::getPSCBuffer() bitwise_const
{
    return m_pixelShaderCBuffer;
}

/* ---------------
   Private Methods
   --------------- */
void
Shader::createVertexPixelShaders(cstring shaderName)
{
     comptr<ID3D10Blob> errorMessage;
    
    // Form the wide vertex shader path
    std::string vertexPath(shaderName);
    vertexPath = "assets/shaders/v_" + vertexPath + ".hlsl";

    // Shader Compilation
    HRESULT result; 
    result = D3DCompileFromFile(string_utils::getwstring(vertexPath).c_str(),
                                nullptr,
                                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                "main",
                                "vs_5_0",
                                D3D10_SHADER_ENABLE_STRICTNESS,                                
                                NULL,
                                &m_vsbuffer,
                                &errorMessage);
    if (FAILED(result))
    {
        // If file was found then there should be a compilation
        // error description
        if (errorMessage)
        {            
            cstring errorDesc = (cstring) errorMessage->GetBufferPointer();
            MessageBox(nullptr, errorDesc, "Failed to compile shader", MB_ICONERROR);
            return;
        }
        // File not found
        else
        {
            std::string missingFile = "Missing shader file: " + vertexPath;
            MessageBox(nullptr, missingFile.c_str(), "Failed to find shader", MB_ICONERROR);
            return;
        }
    }

    // Vertex shader creation
    Renderer::get()->getDeviceHandle()->CreateVertexShader(m_vsbuffer->GetBufferPointer(),
                                                           m_vsbuffer->GetBufferSize(),
                                                           nullptr,
                                                           &m_vertexShader);

    // Form the wide pixel shader path
    std::string pixelPath(shaderName);
    pixelPath = "assets/shaders/p_" + pixelPath + ".hlsl";
    
    // Shader Compilation
    result = D3DCompileFromFile(string_utils::getwstring(pixelPath).c_str(),
                                nullptr,
                                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                "main",
                                "ps_5_0",
                                D3D10_SHADER_ENABLE_STRICTNESS,
                                NULL,
                                &m_psbuffer,
                                &errorMessage);
    if (FAILED(result))
    {
        // If file was found then there should be a compilation
        // error description
        if (errorMessage)
        {
            cstring errorDesc = (cstring) errorMessage->GetBufferPointer();
            MessageBox(nullptr, errorDesc, "Failed to compile shader", MB_ICONERROR);
            return;
        }
        // File not found
        else
        {
            std::string missingFile = "Missing shader file: " + pixelPath;
            MessageBox(nullptr, missingFile.c_str(), "Failed to find shader", MB_ICONERROR);
            return;
        }
    }

    // Pixel shader creation
    Renderer::get()->getDeviceHandle()->CreatePixelShader(m_psbuffer->GetBufferPointer(),
                                                          m_psbuffer->GetBufferSize(),
                                                          nullptr,
                                                          &m_pixelShader);    
}

void
Shader::createConstantBuffers()
{
    // Vertex Shader Constant Buffer description
    D3D11_BUFFER_DESC vsCBufferDesc = {};
    vsCBufferDesc.Usage             = D3D11_USAGE_DEFAULT;
    vsCBufferDesc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
    vsCBufferDesc.ByteWidth         = sizeof(VSCBuffer);
    
    // Vertex Shader Constant Buffer creation
    HR(Renderer::get()->getDeviceHandle()->CreateBuffer(&vsCBufferDesc,
                                                        nullptr,
                                                        &m_vertexShaderCBuffer));

    // Pixel Shader Constant Buffer description
    D3D11_BUFFER_DESC psCBufferDesc = {};
    psCBufferDesc.Usage             = D3D11_USAGE_DEFAULT;
    psCBufferDesc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
    psCBufferDesc.ByteWidth         = sizeof(PSCBuffer);

    // Pixel Shader Constant Buffer creation
    HR(Renderer::get()->getDeviceHandle()->CreateBuffer(&psCBufferDesc, 
                                                        nullptr,
                                                        &m_pixelShaderCBuffer));    
}

void
Shader::createShaderLayout()
{
    // Position Input Layout description
    D3D11_INPUT_ELEMENT_DESC inputPositionDesc = {};    
    inputPositionDesc.Format                   = DXGI_FORMAT_R32G32B32_FLOAT;
    inputPositionDesc.SemanticName             = "POSITION";
    inputPositionDesc.InputSlotClass           = D3D11_INPUT_PER_VERTEX_DATA;
    inputPositionDesc.AlignedByteOffset        = 0U;

    // Texcoord Input Layout description
    D3D11_INPUT_ELEMENT_DESC inputTexcoordDesc = {};
    inputTexcoordDesc.Format                   = DXGI_FORMAT_R32G32_FLOAT;
    inputTexcoordDesc.SemanticName             = "TEXCOORD";
    inputTexcoordDesc.InputSlotClass           = D3D11_INPUT_PER_VERTEX_DATA;
    inputTexcoordDesc.AlignedByteOffset        = D3D11_APPEND_ALIGNED_ELEMENT;

    // Normal Input Layout description
    D3D11_INPUT_ELEMENT_DESC inputNormalDesc   = {};
    inputNormalDesc.Format                     = DXGI_FORMAT_R32G32B32_FLOAT;
    inputNormalDesc.SemanticName               = "NORMAL";
    inputNormalDesc.InputSlotClass             = D3D11_INPUT_PER_VERTEX_DATA;
    inputNormalDesc.AlignedByteOffset          = D3D11_APPEND_ALIGNED_ELEMENT;

    // Combined Input Layout Array
    D3D11_INPUT_ELEMENT_DESC combinedLayout[]  = {inputPositionDesc,
                                                  inputTexcoordDesc,
                                                  inputNormalDesc};
    // Create Shader Input Layout
    HR(Renderer::get()->getDeviceHandle()->CreateInputLayout(combinedLayout,
                                                             ARRAYSIZE(combinedLayout),
                                                             m_vsbuffer->GetBufferPointer(),
                                                             m_vsbuffer->GetBufferSize(),
                                                             &m_shaderInputLayout));
    
    // Set the global input layout
    Renderer::get()->getDevconHandle()->IASetInputLayout(m_shaderInputLayout.Get());
}