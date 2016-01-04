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
#include "texture.h"
#include "../util/stringutils.h"
#include "../window.h"
#include "../game/scene.h"
#include <fstream>

/* -------------
   External Vars
   ------------- */
extern Window* g_window;

/* ----------------
   Internal Structs
   ---------------- */
struct VertexPos { real32 x, y, z;    };
struct VertexTex { real32 tu, tv;     };
struct VertexNor { real32 nx, ny, nz; };
struct ObjIndex  { uint32 vertexIndex, texIndex, normIndex;  };
   
/* -------------------
   Internal Signatures
   ------------------- */
static bool
loadMesh(std::ifstream&             file,
         std::vector<VertexPos>&    disorgPos,
         std::vector<VertexTex>&    disorgTexcoords,
         std::vector<VertexNor>&    disorgNormals,
         std::vector<ObjIndex>&     objIndices,
         std::vector<Mesh::Vertex>& finalVertices,
         std::vector<uint32>&       finalIndices);

/* --------------
   Public Methods
   -------------- */
Mesh::Mesh(cstring meshName,
           uint32  meshCreationFlags,
           Vertex* optExternalVertexData /* nullptr */,
           uint32* optExternalIndexData  /* nullptr */,
           uint32  optExternalNVertices  /* 0U */,
           uint32  optExternalNIndices   /* 0U */):

           m_name(internString(meshName)),
           m_hudElement((meshCreationFlags & MESH_TYPE_HUD) != 0),
           m_hasTexture((meshCreationFlags & MESH_LOAD_SAME_TEXTURE) != 0),
           x(0.0f), y(0.0f), z(0.0f),
           rotX(0.0f), rotY(0.0f), rotZ(0.0f),
           scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f),
           m_dimensions(),
           m_collSPhere(vec3f(), real32()),
           m_visiSphere(vec3f(), real32())
{
    // If there is a mesh registered with this name initialize
    // this mesh from the registered mesh
    if (resource::meshExists(m_name)) init(resource::retrieveMesh(m_name));

    // otherwise initialize this mesh and its texture (if the same texture flag is set)
    // normally.
    else
    {  
        // Same texture flag is set
        if (m_hasTexture) m_texture.reset(new Texture(meshName));            
        
        // Load mesh and if succesful register it
        if ((meshCreationFlags & MESH_EXTERNAL_DATA) != 0)
        {
            createMesh(optExternalVertexData,
                       optExternalIndexData,
                       optExternalNVertices,
                       optExternalNIndices);
            // No register here as it is a mesh with specific data
        }
        else
        {
            if (createMesh(nullptr, nullptr, 0U, 0U)) 
                resource::registerMesh(m_name, this);
        }        
    }
}

Mesh::~Mesh()
{
    Scene::get()->removeMesh(this);
}

void
Mesh::init(const Mesh* rhs)
{
    m_name = rhs->m_name;
    x      = rhs->x;           y = rhs->y;           z = rhs->z;
    rotX   = rhs->rotX;     rotY = rhs->rotY;     rotZ = rhs->rotZ;
    scaleX = rhs->scaleX; scaleY = rhs->scaleY; scaleZ = rhs->scaleZ;

    m_hudElement   = rhs->isHUDElement();
    m_hasTexture   = rhs->hasTexture();
    m_vertexBuffer = rhs->getVertexBuffer();
    m_indexBuffer  = rhs->getIndexBuffer();
    m_texture      = rhs->getTexture();
    m_dimensions   = rhs->calculateDimensions();
    m_indexCount   = rhs->getIndexCount();
}

void
Mesh::loadNewTexture(const cstring textureName)
{
    m_texture.reset(new Texture(textureName));
    m_hasTexture = true;
}

stringID
Mesh::getNameID() logical_const
{
    return m_name;
}

cstring
Mesh::getName() logical_const
{
    return retrieveString(m_name);
}

bool
Mesh::isHUDElement() logical_const
{
    return m_hudElement;
}

bool
Mesh::hasTexture() logical_const
{
    return m_hasTexture;
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
    D3DXMatrixScaling(&scaMatrix, m_hudElement ? scaleX / g_window->getAspect() : scaleX, scaleY, scaleZ);
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

vec3f
Mesh::calculateDimensions() logical_const
{
    return vec3f(m_dimensions.x * scaleX,
                 m_dimensions.y * scaleY,
                 m_dimensions.z * scaleZ);
}

vec3f
Mesh::getPosition() logical_const
{
    return vec3f(x, y, z);
}

math::Geometry&
Mesh::getCollidableGeometry() bitwise_const
{
    vec3f meshDimensions = calculateDimensions();
    m_collSPhere.setPosition(vec3f(x, y, z));
    m_collSPhere.setRadius((math::avg3f(meshDimensions.x,
                                        meshDimensions.y,
                                        meshDimensions.z)) / 2.0f);
    return m_collSPhere;
}

math::Geometry&
Mesh::getVisibleGeometry() bitwise_const
{
    vec3f meshDimensions = calculateDimensions();
    m_visiSphere.setPosition(vec3f(x, y, z));
    m_visiSphere.setRadius((math::max3f(meshDimensions.x,
                                        meshDimensions.y,
                                        meshDimensions.z)) / 2.0f);
    return m_visiSphere;
}

std::shared_ptr<Texture>
Mesh::getTexture() bitwise_const
{
    return m_texture;
}

void
Mesh::setTexture(std::shared_ptr<Texture> texture)
{
    m_texture = texture;
}

/* ---------------
   Private Methods
   --------------- */

bool
Mesh::createMesh(Vertex* optVertices,
                 uint32* optIndices,
                 uint32  optNVertices,
                 uint32  optNIndices)
{
    // Disorganized data containers
    std::vector<VertexPos> disorgPos;
    std::vector<VertexTex> disorgTexcoords;
    std::vector<VertexNor> disorgNormals;
    std::vector<ObjIndex> objIndices;

    // Organized data containers
    std::vector<Vertex> finalVertices;
    std::vector<uint32> finalIndices;

    // Check if external vertex and index data are provided
    if (optVertices)
    {
        while (finalVertices.size() < optNVertices)
        {
            finalVertices.push_back(*optVertices++);            
        }
        while (finalIndices.size() < optNIndices)
        {
            finalIndices.push_back(*optIndices++);            
        }       
    }
    
    // Otherwise begin manual mesh loading
    else
    {
        std::string meshName = retrieveString(m_name);
        std::ifstream file("assets/models/" + meshName + ".obj");

        if (!file.is_open())
        {
            MessageBox(NULL,
                       ("Missing mesh file: " + meshName).c_str(),
                       "Mesh Missing!",
                       MB_ICONEXCLAMATION);
            return false;
        }

        if (!loadMesh(file,
                      disorgPos,
                      disorgTexcoords, 
                      disorgNormals, 
                      objIndices, 
                      finalVertices,
                      finalIndices)) return false;
    }
    
    real32 minWidth  = 0.0f, maxWidth  = 0.0f;
    real32 minDepth  = 0.0f, maxDepth  = 0.0f;
    real32 minHeight = 0.0f, maxHeight = 0.0f;

    // Update Dimensions
    for (auto citer = finalVertices.cbegin();
         citer != finalVertices.cend();
         ++citer)
    {
        if (citer->x < minWidth)  minWidth  = citer->x;
        if (citer->x > maxWidth)  maxWidth  = citer->x;
        if (citer->y < minHeight) minHeight = citer->y;
        if (citer->y > maxHeight) maxHeight = citer->y;
        if (citer->z < minDepth)  minDepth  = citer->z;
        if (citer->z > maxDepth)  maxDepth  = citer->z;
    }
    m_dimensions.x = std::abs(minWidth)  + std::abs(maxWidth);
    m_dimensions.y = std::abs(minHeight) + std::abs(maxHeight);
    m_dimensions.z = std::abs(minDepth)  + std::abs(maxDepth);

    m_indexCount = finalIndices.size();
    
    // Vertex Buffer Creation
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * finalVertices.size();

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = &finalVertices[0];

    HR(Renderer::get()->getDeviceHandle()->CreateBuffer(&vertexBufferDesc,
                                                        &vertexData,
                                                        &m_vertexBuffer));

    // Index Buffer creation
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.ByteWidth = sizeof(uint32) * finalIndices.size();

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = &finalIndices[0];

    HR(Renderer::get()->getDeviceHandle()->CreateBuffer(&indexBufferDesc,
                                                        &indexData,
                                                        &m_indexBuffer));

    return true;
}

static bool
loadMesh(std::ifstream&             file,
         std::vector<VertexPos>&    disorgPos,
         std::vector<VertexTex>&    disorgTexcoords,
         std::vector<VertexNor>&    disorgNormals,
         std::vector<ObjIndex>&     objIndices,
         std::vector<Mesh::Vertex>& finalVertices,
         std::vector<uint32>&       finalIndices)
{
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
                if (splitIndices[1].size() != 0) objIndex.texIndex = std::stoi(splitIndices[1]) - 1;
                if (splitIndices[2].size() != 0) objIndex.normIndex = std::stoi(splitIndices[2]) - 1;

                objIndices.push_back(objIndex);
            }
        }
    }
    file.close();

    // Calculate the final data
    for (size_t i = 0; i < objIndices.size(); ++i)
    {
        Mesh::Vertex finalVertex = {};

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

    return true;
}