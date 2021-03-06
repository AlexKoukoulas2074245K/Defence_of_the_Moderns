/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        mesh.h

   File description: A class describing all
   the properties of a mesh
   ------------------------------------------ */

#pragma once

#include "d3d11common.h"
#include "../util/math.h"
#include "../dotmdef.h"
#include "../util/strings.h"
#include "shader.h"
#include <memory>

class Entity;
class Texture;
class Scene;
class Mesh
{
public:

    struct Vertex    
    {
        real32 v_x, v_y, v_z;
        real32 v_tu, v_tv;
        real32 v_nx, v_ny, v_nz;
    };

public:

    static const uint32 MESH_TYPE_NORMAL        = 0x01;
    static const uint32 MESH_TYPE_HUD           = 0x02;
    static const uint32 MESH_LOAD_SAME_TEXTURE  = 0x04;
    static const uint32 MESH_EXTERNAL_TEXCOORDS = 0x08;
    static const uint32 MESH_TYPE_BILLBOARD     = 0x10;

private:

    static const Vertex MESH_HUD_VERTICES[4];
    static const uint32 MESH_HUD_INDICES[6];

public:

    // <summary>
    // <para> 
    //  If the optSceneptr parameter is set, the mesh
    // will attempt to remove itself from that scene during destruction
    // If the MESH_EXTERNAL_TEXCOORDS flag is set,
    // the method expects a valid array of vec2f and its size.    
    // </para>
    // </summary>
    Mesh(cstring meshName,
         uint32  meshCreationFlags,
         vec2f*  optExternalCoords  = nullptr,
         uint32  optNExternalCoords = 0U);

    ~Mesh();

    Mesh(const Mesh& rhs);

    Mesh&
    operator = (const Mesh& rhs);

    void
    loadNewTexture(cstring textureName);

    stringID
    getNameID() logical_const;

    bool 
    isHUDElement() logical_const;
    
    bool
    isBillboard() logical_const;

    bool
    noLighting() logical_const;

    mat4x4
    getWorldMatrix() logical_const;

    mat4x4
    getTranslationMatrix() logical_const;

    mat4x4
    getScaleMatrix() logical_const;

    mat4x4
    getRotationMatrix() logical_const;

    uint32
    getIndexCount() logical_const;

    comptr<ID3D11Buffer>
    getVertexBuffer() bitwise_const;

    comptr<ID3D11Buffer>
    getIndexBuffer() bitwise_const;

    Shader::VSCBuffer*
    getSpecialVCBuffer() bitwise_const;

    Shader::PSCBuffer*
    getSpecialPCBuffer() bitwise_const;

    vec3f
    calculateDimensions() logical_const;

    std::shared_ptr<Texture>
    getTexture() bitwise_const;
    
    math::Geometry&
    getCollidableGeometry() bitwise_const;

    math::Geometry&
    getVisibleGeometry() bitwise_const;

    bool
    isHighlighted() logical_const;

    void
    setHighlighted(const bool highlighted);

    void
    setTexture(std::shared_ptr<Texture> texture);

    void
    setNoLighting(const bool noLighting);

    void
    setSpecialVCBuffer(Shader::VSCBuffer* vscbuffer);

    void
    setSpecialPCBuffer(Shader::PSCBuffer* pscbuffer);

private:

    bool
    createMesh(vec2f*  optTexCoords,               
               uint32  optNTexCoords);

public: 
    // the prefix m_ in the public fields is intentionally not added
    // to enable faster field accessing and a struct like use    
    vec3f position, rotation, scale;
   
private:

    stringID m_name;
    uint32   m_meshFlags;
    uint32   m_indexCount;    
    vec3f    m_dimensions;
    bool     m_highlighted;    
    bool     m_noLighting;

    Shader::VSCBuffer*       m_specialVCBuffer;
    Shader::PSCBuffer*       m_specialPCBuffer;
    comptr<ID3D11Buffer>     m_vertexBuffer;
    comptr<ID3D11Buffer>     m_indexBuffer;
    std::shared_ptr<Texture> m_texture;
    
    mutable math::Sphere m_collSPhere;
    mutable math::Sphere m_visiSphere;
};