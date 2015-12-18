/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        mesh.cpp

   File description: Implementation of the 
   Mesh class declared in mesh.h
   ------------------------------------------ */

#include "mesh.h"
#include "meshregistry.h"

/* --------------
   Public Methods
   -------------- */
Mesh::Mesh(cstring meshName, bool isHUDElement):

    name(internString(meshName)),
    m_hudElement(isHUDElement),
    x(0.0f), y(0.0f), z(0.0f),
    rotX(0.0f), rotY(0.0f), rotZ(0.0f),
    scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f)
{
    if (mr_entryExists(name)) init(mr_retrieveMesh(name));

}

Mesh::~Mesh()
{

}

void
Mesh::init(const Mesh* rhs)
{
    name   = rhs->name;
    x      = rhs->x;           y = rhs->y;           z = rhs->z;
    rotX   = rhs->rotX;     rotY = rhs->rotY;     rotZ = rhs->rotZ;
    scaleX = rhs->scaleX; scaleY = rhs->scaleY; scaleZ = rhs->scaleZ;

    m_hudElement = rhs->isHUDElement();

    m_vertexBuffer = rhs->getVertexBuffer();
    m_indexBuffer  = rhs->getIndexBuffer();
}

bool
Mesh::isHUDElement() logical_const
{
    return m_hudElement;
}

mat4x4
Mesh::getFinalMatrix(const mat4x4& view,
                     const mat4x4& proj) logical_const
{
    if (m_hudElement) return getWorldMatrix();
    else return getWorldMatrix() * view * proj;
}

mat4x4 
Mesh::getWorldMatrix() logical_const
{
    return getScaleMatrix() *
           getRotationMatrix() *
           getTranslationMatrix();
}

mat4x4 
Mesh::getTranslationMatrix() logical_const
{
    D3DXMATRIX trMatrix;
    D3DXMatrixTranslation(&trMatrix, x, y, z);
    return trMatrix;
}

mat4x4
Mesh::getRotationMatrix() logical_const
{
    D3DXMATRIX rotXMatrix, rotYMatrix, rotZMatrix;
    D3DXMatrixRotationX(&rotXMatrix, rotX);
    D3DXMatrixRotationY(&rotYMatrix, rotY);
    D3DXMatrixRotationZ(&rotZMatrix, rotZ);
    return rotXMatrix * rotYMatrix * rotZMatrix;
}

mat4x4 
Mesh::getScaleMatrix() logical_const
{
    D3DXMATRIX scaMatrix;
    D3DXMatrixScaling(&scaMatrix, scaleX, scaleY, scaleZ);
    return scaMatrix;
}

comptr<ID3D11Buffer>
Mesh::getVertexBuffer() bitwise_const
{
    return m_vertexBuffer;
}

comptr<ID3D11Buffer>
Mesh::getIndexBuffer() bitwise_const
{
    return m_indexBuffer;
}