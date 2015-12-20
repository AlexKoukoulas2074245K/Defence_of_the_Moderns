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

    Mesh(cstring meshName, bool isHUDElement);

    ~Mesh();

    Mesh(const Mesh& rhs);

    Mesh&
    operator = (const Mesh& rhs);

    void 
    init(const Mesh* rhs);

    stringID
    getName() logical_const;

    bool 
    isHUDElement() logical_const;

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

    comptr<ID3D11ShaderResourceView>
    getTexture() bitwise_const;

private:

    bool
    loadMesh();

    bool
    loadTexture();

public: 
    // the prefix m_ in the public fields is intentionally not added
    // to enable faster field accessing and a struct like use    
    real32 x, y, z;
    real32 rotX, rotY, rotZ;
    real32 scaleX, scaleY, scaleZ;
   
private:

    stringID m_name;
    bool m_hudElement;
    uint32 m_indexCount;

    comptr<ID3D11Buffer> m_vertexBuffer;
    comptr<ID3D11Buffer> m_indexBuffer;
    comptr<ID3D11ShaderResourceView> m_texture;
};