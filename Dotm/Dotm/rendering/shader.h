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
        mat4x4 vcb_rotationMatrix;
        mat4x4 vcb_worldMatrix;
        mat4x4 vcb_mvpMatrix;
        vec4f  vcb_eyePosition;
        int32  vcb_highlight;
        vec3f  _padding;
    };

    struct DirectionalLight
    {
        vec4f  dl_ambientColor;
        vec4f  dl_diffuseColor;
        vec3f  dl_direction;
        real32 _padding;
    };

    struct PointLight
    {
        vec4f  pl_ambientColor;
        vec4f  pl_diffuseColor;
        vec3f  pl_position;
        real32 pl_range;        
    };

   struct PSCBuffer
    {
        DirectionalLight pcb_directionalLights[SHADER_MAX_DIRECTIONAL_LIGHTS];
        PointLight       pcb_pointLights[SHADER_MAX_POINT_LIGHTS];

        void clear() 
        {
            for (size_t i = 0; i < SHADER_MAX_DIRECTIONAL_LIGHTS; ++i)
            {
                pcb_directionalLights[i].dl_ambientColor = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
                pcb_directionalLights[i].dl_diffuseColor = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
                pcb_directionalLights[i].dl_direction = vec3f(0.0f, 0.0f, 0.0f);
            }
            for (size_t i = 0; i < SHADER_MAX_POINT_LIGHTS; ++i)
            {
                pcb_pointLights[i].pl_ambientColor = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
                pcb_pointLights[i].pl_diffuseColor = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
                pcb_pointLights[i].pl_position = vec3f(0.0f, 0.0f, 0.0f);
                pcb_pointLights[i].pl_range = 0.0f;
            }
        }
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