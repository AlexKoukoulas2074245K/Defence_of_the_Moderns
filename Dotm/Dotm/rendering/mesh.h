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

    Mesh(cstring meshName, bool isHUDElement);

    ~Mesh();

    Mesh(const Mesh& rhs);

    Mesh&
    operator = (const Mesh& rhs);

    void
    init(const Mesh& rhs);

    void 
    init(const Mesh* rhs);

    bool 
    isHUDElement() logical_const;

    mat4x4
    getFinalMatrix(const mat4x4& view,
                   const mat4x4& proj) logical_const;

    mat4x4
    getWorldMatrix() logical_const;

    mat4x4
    getTranslationMatrix() logical_const;

    mat4x4
    getScaleMatrix() logical_const;

    mat4x4
    getRotationMatrix() logical_const;

    comptr<ID3D11Buffer>
    getVertexBuffer() bitwise_const;

    comptr<ID3D11Buffer>
    getIndexBuffer() bitwise_const;

public: 
    // the prefix m_ in the public fields is intentionally not added
    // to enable faster field accessing and a struct like use
    stringID name;
    real32 x, y, z;
    real32 rotX, rotY, rotZ;
    real32 scaleX, scaleY, scaleZ;
   
private:

    comptr<ID3D11Buffer> m_vertexBuffer;
    comptr<ID3D11Buffer> m_indexBuffer;
    bool m_hudElement;

};