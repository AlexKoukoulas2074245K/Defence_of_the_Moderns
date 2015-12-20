/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        mesh.cpp

   File description: Implementation of the 
   Mesh class declared in mesh.h
   ------------------------------------------ */

#include "mesh.h"
#include "resregistry.h"
#include "renderer.h"
#include "ddsloader.h"
#include "../util/stringutils.h"
#include <fstream>

/* --------------
   Public Methods
   -------------- */
Mesh::Mesh(cstring meshName, bool isHUDElement):

    m_name(internString(meshName)),
    m_hudElement(isHUDElement),
    x(0.0f), y(0.0f), z(0.0f),
    rotX(0.0f), rotY(0.0f), rotZ(0.0f),
    scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f)
{
    // If there is a mesh registered with this name initialize
    // this mesh from the registered mesh
    if (resource::meshExists(m_name)) init(resource::retrieveMesh(m_name));

    // otherwise if the mesh loading is successful 
    // register this mesh with its name
    else if(loadMesh() & loadTexture()) resource::registerMesh(m_name, this);
}

Mesh::~Mesh()
{

}

void
Mesh::init(const Mesh* rhs)
{
    m_name = rhs->m_name;
    x      = rhs->x;           y = rhs->y;           z = rhs->z;
    rotX   = rhs->rotX;     rotY = rhs->rotY;     rotZ = rhs->rotZ;
    scaleX = rhs->scaleX; scaleY = rhs->scaleY; scaleZ = rhs->scaleZ;

    m_hudElement = rhs->isHUDElement();

    m_vertexBuffer = rhs->getVertexBuffer();
    m_indexBuffer  = rhs->getIndexBuffer();
}

stringID
Mesh::getName() logical_const
{
    return m_name;
}

bool
Mesh::isHUDElement() logical_const
{
    return m_hudElement;
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

uint32
Mesh::getIndexCount() logical_const
{
    return m_indexCount;
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

comptr<ID3D11ShaderResourceView>
Mesh::getTexture() bitwise_const
{
    return m_texture;
}

/* ---------------
   Private Methods
   --------------- */

struct VertexPos { real32 x, y, z;    };
struct VertexTex { real32 tu, tv;     };
struct VertexNor { real32 nx, ny, nz; };
struct ObjIndex  { uint32 vertexIndex, texIndex, normIndex;  };

bool
Mesh::loadMesh()
{
    std::string meshName = retrieveString(m_name);
    std::ifstream file("models/" + meshName + ".obj");

    if (!file.is_open())
    {
        MessageBox(NULL,
                   ("Missing mesh file: " + meshName).c_str(),
                   "Mesh Missing!",
                   MB_ICONEXCLAMATION);
        return false;
    }

    // Disorganized data containers
    std::vector<VertexPos> disorgPos;
    std::vector<VertexTex> disorgTexcoords;
    std::vector<VertexNor> disorgNormals;
    std::vector<ObjIndex> objIndices;

    std::string line;
    for (; std::getline(file, line);)
    {
        if (line.size() < 2 ||
            (line[0] != 'v' && line[0] != 'f')) continue;
        
        std::vector<std::string> vecComps;
        string_utils::split(line, ' ', vecComps);
        std::string attribute = vecComps[0];

        // vertex
        if (attribute[0] == 'v')
        {
            // texcoord
            if (attribute[1] == 't')
            {
                VertexTex texcoord = {};
                texcoord.tu = std::stof(vecComps[1]);
                texcoord.tv = 1 - std::stof(vecComps[2]);
                disorgTexcoords.push_back(texcoord);
            }
            // normal
            else if (attribute[1] == 'n')
            {
                VertexNor normal = {};
                normal.nx = std::stof(vecComps[1]);
                normal.ny = std::stof(vecComps[2]);
                normal.nz = std::stof(vecComps[3]);
                disorgNormals.push_back(normal);
            }
            // position
            else
            {
                VertexPos position = {};
                position.x = std::stof(vecComps[1]);
                position.y = std::stof(vecComps[2]);
                position.z = std::stof(vecComps[3]);
                disorgPos.push_back(position);
            }
        }
        // face
        else
        {
            for (size_t i = 1; i < 4; ++i)
            {
                ObjIndex objIndex = {};

                std::vector<std::string> splitIndices;
                string_utils::split(vecComps[i], '/', splitIndices);

                if (splitIndices[0].size() != 0) objIndex.vertexIndex = std::stoi(splitIndices[0]) - 1;
                if (splitIndices[1].size() != 0) objIndex.texIndex    = std::stoi(splitIndices[1]) - 1;
                if (splitIndices[2].size() != 0) objIndex.normIndex   = std::stoi(splitIndices[2]) - 1;

                objIndices.push_back(objIndex);
            }
        }
    }
    file.close();

    // Organized data containers
    std::vector<Vertex> finalVertices;
    std::vector<uint32> finalIndices;

    for (size_t i = 0; i < objIndices.size(); ++i)
    {
        Vertex finalVertex = {};
        
        // Position Extraction        
        finalVertex.x = disorgPos[objIndices[i].vertexIndex].x;
        finalVertex.y = disorgPos[objIndices[i].vertexIndex].y;
        finalVertex.z = disorgPos[objIndices[i].vertexIndex].z;        
    
        // Tex Coord Extraction
        finalVertex.tu = disorgTexcoords[objIndices[i].texIndex].tu;
        finalVertex.tv = disorgTexcoords[objIndices[i].texIndex].tv;
                        
        // Normal Extraction        
        finalVertex.nx = disorgNormals[objIndices[i].normIndex].nx;
        finalVertex.ny = disorgNormals[objIndices[i].normIndex].ny;
        finalVertex.nz = disorgNormals[objIndices[i].normIndex].nz;
              
        finalVertices.push_back(finalVertex);
        finalIndices.push_back(i);
    }
    
    m_indexCount = finalIndices.size();

    // Vertex Buffer Creation
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * finalVertices.size();

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = &finalVertices[0];

    HR(Renderer::get().getDeviceHandle()->CreateBuffer(&vertexBufferDesc,
                                                       &vertexData,
                                                       &m_vertexBuffer));

    // Index Buffer creation
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof(uint32) * finalIndices.size();

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = &finalIndices[0];

    HR(Renderer::get().getDeviceHandle()->CreateBuffer(&indexBufferDesc,
                                                       &indexData,
                                                       &m_indexBuffer));

    return true;
}

bool
Mesh::loadTexture()
{
    std::wstring widePath = L"textures/" + 
                            string_utils::getwstring(retrieveString(m_name)) +
                            L".png";

    HRESULT result = DirectX::CreateWICTextureFromFile(
        Renderer::get().getDeviceHandle().Get(),
        Renderer::get().getDevconHandle().Get(),
        widePath.c_str(),
        NULL,
        &m_texture,
        0);
    
    if (FAILED(result))
    {        
        DirectX::CreateWICTextureFromFile(
            Renderer::get().getDeviceHandle().Get(),
            Renderer::get().getDevconHandle().Get(),
            L"textures/missing.png",
            NULL,
            &m_texture,
            0);
        
        MessageBoxW(NULL,
                    (L"Missing texture: " + widePath).c_str(),
                    L"Missing Texture!",
                    MB_ICONEXCLAMATION);
    }

    return true;
}