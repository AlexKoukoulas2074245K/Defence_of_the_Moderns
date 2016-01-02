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
#include <memory>

class Texture;
class Mesh
{
public:

    struct Vertex    
    {
        real32 x, y, z;
        real32 tu, tv;
        real32 nx, ny, nz;
    };

public:

    static const uint32 MESH_TYPE_NORMAL       = 0x01;
    static const uint32 MESH_TYPE_HUD          = 0x02;
    static const uint32 MESH_LOAD_SAME_TEXTURE = 0x04;
    static const uint32 MESH_EXTERNAL_DATA     = 0x08;

public:

    // If the MESH_EXTERNAL_TEXCOORDS flag is set,
    // the method expects a valid array of vec2f and its size
    Mesh(cstring meshName,
         uint32  meshCreationFlags,
         Vertex* optExternalVertexData = nullptr,
         uint32* optExternalIndexData  = nullptr,
         uint32  optExternalNVertices  = 0U,
         uint32  optExternalNIndices   = 0U);

    ~Mesh();

    Mesh(const Mesh& rhs);

    Mesh&
    operator = (const Mesh& rhs);

    void 
    init(const Mesh* rhs);

    void
    loadNewTexture(cstring textureName);

    stringID
    getNameID() logical_const;

    cstring
    getName() logical_const;

    bool 
    isHUDElement() logical_const;

    bool
    hasTexture() logical_const;

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

    std::shared_ptr<Texture>
    getTexture() bitwise_const;
    
    vec3f
    getDimensions() logical_const;

    vec3f
    getPosition() logical_const;

    void
    setTexture(std::shared_ptr<Texture> texture);

private:

    bool
    createMesh(Vertex* optVertices,
               uint32* optIndices,
               uint32  optNVertices,
               uint32  optNIndices);

public: 
    // the prefix m_ in the public fields is intentionally not added
    // to enable faster field accessing and a struct like use    
    real32 x, y, z;
    real32 rotX, rotY, rotZ;
    real32 scaleX, scaleY, scaleZ;
   
private:

    stringID m_name;
    
    bool   m_hudElement;
    bool   m_hasTexture;
    uint32 m_indexCount;
    vec3f  m_dimensions;

    comptr<ID3D11Buffer>     m_vertexBuffer;
    comptr<ID3D11Buffer>     m_indexBuffer;
    std::shared_ptr<Texture> m_texture;
};