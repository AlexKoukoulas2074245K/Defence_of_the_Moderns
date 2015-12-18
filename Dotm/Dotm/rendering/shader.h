/* -------------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        shader.h

   File description: An class encapsulating the base
   functionality of a shader.
   ------------------------------------------------- */

#pragma once

#include "d3d11common.h"
#include "../util/strings.h"
#include "../util/math.h"
#include "../dotmdef.h"

class Shader
{
public:

    struct VSCBuffer
    {
        mat4x4 rotationMatrix;
        mat4x4 worldMatrix;
        mat4x4 mvpMatrix;
        vec4   eyePosition;
    };

    struct PSCBuffer
    {
        real32 _padding;
        real32 _padding1;
        real32 _padding2;
        real32 _padding3;
    };

public:

    Shader(comptr<ID3D11Device> device,
           cstring shaderName);

    ~Shader();

    Shader(const Shader& rhs) = delete;

    Shader&
    operator = (const Shader& rhs) = delete;

    bool
    isReady() logical_const;

    comptr<ID3D11InputLayout>
    getShaderLayout() bitwise_const;

    comptr<ID3D11VertexShader>
    getVertexShader() bitwise_const;

    comptr<ID3D11PixelShader>
    getPixelShader() bitwise_const;

    comptr<ID3D11Buffer>
    getVSCBuffer() bitwise_const;

    comptr<ID3D11Buffer>
    getPSCBuffer() bitwise_const;

private:

    bool
    createVertexPixelShaders(comptr<ID3D11Device> device,
                             cstring shaderName);

    bool 
    createConstantBuffers(comptr<ID3D11Device> device);

    bool
    createShaderLayout(comptr<ID3D11Device> device);

protected:

    comptr<ID3D10Blob>         m_vsbuffer;
    comptr<ID3D10Blob>         m_psbuffer;
    comptr<ID3D11VertexShader> m_vertexShader;
    comptr<ID3D11PixelShader>  m_pixelShader;
    comptr<ID3D11InputLayout>  m_shaderInputLayout;
    comptr<ID3D11Buffer>       m_vertexShaderCBuffer;
    comptr<ID3D11Buffer>       m_pixelShaderCBuffer;

    bool ready;
};