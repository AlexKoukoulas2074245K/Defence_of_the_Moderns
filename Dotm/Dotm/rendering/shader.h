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

    static const uint32 SHADER_MAX_DIRECTIONAL_LIGHTS = 4U;
    static const uint32 SHADER_MAX_POINT_LIGHTS       = 10U;

public:

    struct VSCBuffer
    {
        mat4x4 rotationMatrix;
        mat4x4 worldMatrix;
        mat4x4 mvpMatrix;
        vec4f  eyePosition;
    };

    struct DirectionalLight
    {
        vec4f  ambientColor;
        vec4f  diffuseColor;
        vec3f  direction;
        real32 _padding;
    };

    struct PointLight
    {
        vec4f  ambientColor;
        vec4f  diffuseColor;
        vec3f  position;
        real32 range;        
    };

   struct PSCBuffer
    {
        DirectionalLight directionalLights[SHADER_MAX_DIRECTIONAL_LIGHTS];
        PointLight       pointLights[SHADER_MAX_POINT_LIGHTS];
    };

public:

    Shader(cstring shaderName);

    ~Shader();

    Shader(const Shader& rhs) = delete;

    Shader&
    operator = (const Shader& rhs) = delete;

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

    void
    createVertexPixelShaders(cstring shaderName);

    void 
    createConstantBuffers();

    void
    createShaderLayout();

protected:

    comptr<ID3D10Blob>         m_vsbuffer;
    comptr<ID3D10Blob>         m_psbuffer;
    comptr<ID3D11VertexShader> m_vertexShader;
    comptr<ID3D11PixelShader>  m_pixelShader;
    comptr<ID3D11InputLayout>  m_shaderInputLayout;
    comptr<ID3D11Buffer>       m_vertexShaderCBuffer;
    comptr<ID3D11Buffer>       m_pixelShaderCBuffer;

};