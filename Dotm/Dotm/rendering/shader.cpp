/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        shader.cpp

   File description: An implementation of the
   shader class declared in shader.h
   ------------------------------------------------ */

#include "shader.h"
#include <string>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

/* --------------
   Public Methods
   -------------- */
Shader::Shader(comptr<ID3D11Device> device,
               cstring shaderName)
{
    comptr<ID3D10Blob> errorMessage;
    

    // Form the wide vertex shader path
    std::string vertexPath(shaderName);
    vertexPath = "shaders/" + vertexPath + ".vs";
    std::wstring wideVertexPath(vertexPath.begin(), vertexPath.end());

    // Shader Compilation
    HRESULT result; 
    result = D3DCompileFromFile(wideVertexPath.c_str(),
                                NULL,
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
            MessageBox(NULL, errorDesc, "Failed to compile shader", MB_ICONERROR);
            return;
        }
        // File not found
        else
        {
            std::string missingFile = "Missing shader file: " + vertexPath;
            MessageBox(NULL, missingFile.c_str(), "Failed to find shader", MB_ICONERROR);
            return;
        }
    }

    // Vertex shader creation
    device->CreateVertexShader(m_vsbuffer->GetBufferPointer(),
                               m_vsbuffer->GetBufferSize(),
                               NULL,
                               &m_vertexShader);

    // Form the wide pixel shader path
    std::string pixelPath(shaderName);
    pixelPath = "shaders/" + pixelPath + ".ps";
    std::wstring widePixelPath(pixelPath.begin(), pixelPath.end());

    // Shader Compilation
    result = D3DCompileFromFile(widePixelPath.c_str(),
        NULL,
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
            MessageBox(NULL, errorDesc, "Failed to compile shader", MB_ICONERROR);
            return;
        }
        // File not found
        else
        {
            std::string missingFile = "Missing shader file: " + pixelPath;
            MessageBox(NULL, missingFile.c_str(), "Failed to find shader", MB_ICONERROR);
            return;
        }
    }

    // Pixel shader creation
    device->CreatePixelShader(m_psbuffer->GetBufferPointer(),
        m_psbuffer->GetBufferSize(),
        NULL,
        &m_pixelShader);
}

Shader::~Shader()
{

}
