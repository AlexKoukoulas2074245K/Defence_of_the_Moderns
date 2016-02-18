/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             18/12/2015
   File name:        mesh.cpp

   File description: Implementation of the 
   Mesh class declared in mesh.h
   ------------------------------------------ */

#include "mesh.h"
#include "renderer.h"
#include "texture.h"
#include "../util/stringutils.h"
#include "../window.h"
#include "../game/scene.h"
#include <fstream>
#include <unordered_map>

/* -------------
   External Vars
   ------------- */
extern Window* g_window;
   
/* ----------------
   Internal Structs
   ---------------- */
struct VertexPos      { real32 x, y, z;    };
struct VertexTex      { real32 tu, tv;     };
struct VertexNor      { real32 nx, ny, nz; };
struct ObjIndex       { uint32 vertexIndex, texIndex, normIndex;  };

struct MeshCachedData { comptr<ID3D11Buffer> mcd_vertexBuffer;
                        comptr<ID3D11Buffer> mcd_indexBuffer;
                        uint32               mcd_indexCount;
                        vec3f                mcd_dimensions; };

/* -------------
   Internal Vars
   ------------- */
static std::unordered_map<stringID, std::shared_ptr<MeshCachedData>> s_cachedMeshes;

/* -------------------
   Internal Signatures
   ------------------- */
static void
registerMeshData(const stringID meshID,
                 const Mesh* mesh);

static bool
isPresent(const stringID meshID);

static std::shared_ptr<MeshCachedData>
retrieveMeshData(const stringID meshID);

static bool
loadMesh(std::ifstream&             file,
         std::vector<VertexPos>&    disorgPos,
         std::vector<VertexTex>&    disorgTexcoords,
         std::vector<VertexNor>&    disorgNormals,
         std::vector<ObjIndex>&     objIndices,
         std::vector<Mesh::Vertex>& finalVertices,
         std::vector<uint32>&       finalIndices);

   
/* ----------------
   Static Constants
   ---------------- */
const Mesh::Vertex Mesh::MESH_HUD_VERTICES[] =
{
    Mesh::Vertex{-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    Mesh::Vertex{ 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
    Mesh::Vertex{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    Mesh::Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}
};

const uint32 Mesh::MESH_HUD_INDICES[] = 
{
    0, 1, 2, 2, 3, 0
};

/* --------------
   Public Methods
   -------------- */
Mesh::Mesh(cstring      meshName,
           uint32       meshCreationFlags,
           vec2f*       optExternalCoords,   /* nullptr */
           uint32       optNExternalCoords   /* 0U */):

           m_name(internString(meshName)),           
           m_meshFlags(meshCreationFlags),
           position(0.0f, 0.0f, 0.0f),
           rotation(0.0f, 0.0f, 0.0f),
           scale(1.0f, 1.0f, 1.0f),
           m_dimensions(),
           m_collSPhere(vec3f(), real32()),
           m_visiSphere(vec3f(), real32()),
           m_highlighted(false),
           m_noLighting(false)
{
    // If there is a mesh registered with this name initialize
    // this mesh from the registered mesh
    if (isPresent(m_name))
    {
        auto cachedData = retrieveMeshData(m_name);
        m_vertexBuffer  = cachedData->mcd_vertexBuffer;
        m_indexBuffer   = cachedData->mcd_indexBuffer;
        m_indexCount    = cachedData->mcd_indexCount;
        m_dimensions    = cachedData->mcd_dimensions;
    }    
    // otherwise initialize this mesh and its texture (if the same texture flag is set)
    // normally.
    else
    {  
        // Load mesh and if succesful register it
        if ((meshCreationFlags & MESH_EXTERNAL_TEXCOORDS) != 0)
        {
            createMesh(optExternalCoords, optNExternalCoords);
            // No register here as it is a mesh with specific data
        }
        else
        {
            createMesh(nullptr, 0U);
            registerMeshData(m_name, this);
        }        
    }

    // Same texture flag is set
    if ((meshCreationFlags & MESH_LOAD_SAME_TEXTURE) != 0)
    {
        m_texture.reset(new Texture(meshName));
    }
}

Mesh::~Mesh()
{
    
}

void
Mesh::loadNewTexture(const cstring textureName)
{
    m_texture.reset(new Texture(textureName));    
}

stringID
Mesh::getNameID() logical_const
{
    return m_name;
}

bool
Mesh::isHUDElement() logical_const
{
    return (m_meshFlags & MESH_TYPE_HUD) != 0;
}

bool
Mesh::isBillboard() logical_const
{
    return (m_meshFlags & MESH_TYPE_BILLBOARD) != 0;
}

bool
Mesh::noLighting() logical_const
{
    return m_noLighting;
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
    D3DXMatrixTranslation(&trMatrix,
                           position.x,
                           position.y,
                           position.z);
    return trMatrix;
}

mat4x4
Mesh::getRotationMatrix() logical_const
{
    D3DXMATRIX rotXMatrix, rotYMatrix, rotZMatrix;
    D3DXMatrixRotationX(&rotXMatrix, rotation.x);
    D3DXMatrixRotationY(&rotYMatrix, rotation.y);
    D3DXMatrixRotationZ(&rotZMatrix, rotation.z);
    return rotXMatrix * rotYMatrix * rotZMatrix;
}

mat4x4 
Mesh::getScaleMatrix() logical_const
{
    D3DXMATRIX scaMatrix;
    D3DXMatrixScaling(&scaMatrix, isHUDElement() ? scale.x / g_window->getAspect() :
                                                   scale.x, scale.y, scale.z);
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

Shader::VSCBuffer*
Mesh::getSpecialVCBuffer() bitwise_const
{
    return m_specialVCBuffer;
}

Shader::PSCBuffer*
Mesh::getSpecialPCBuffer() logical_const
{
    return m_specialPCBuffer;
}

vec3f
Mesh::calculateDimensions() logical_const
{
    return vec3f(m_dimensions.x * scale.x,
                 m_dimensions.y * scale.y,
                 m_dimensions.z * scale.z);
}

math::Geometry&
Mesh::getCollidableGeometry() bitwise_const
{
    vec3f meshDimensions = calculateDimensions();
    m_collSPhere.setPosition(position);
    m_collSPhere.setRadius((math::avg3f(meshDimensions.x,
                                        meshDimensions.y,
                                        meshDimensions.z)) / 2.0f);
    return m_collSPhere;
}

math::Geometry&
Mesh::getVisibleGeometry() bitwise_const
{
    vec3f meshDimensions = calculateDimensions();
    m_visiSphere.setPosition(position);
    m_visiSphere.setRadius(math::max3f(meshDimensions.x,
                                        meshDimensions.y,
                                        meshDimensions.z));
    return m_visiSphere;
}

std::shared_ptr<Texture>
Mesh::getTexture() bitwise_const
{
    return m_texture;
}

bool
Mesh::isHighlighted() logical_const
{
    return m_highlighted;
}

void
Mesh::setHighlighted(const bool highlighted)
{
    m_highlighted = highlighted;
}

void
Mesh::setTexture(std::shared_ptr<Texture> texture)
{
    m_texture = texture;
}

void
Mesh::setNoLighting(const bool noLighting)
{
    m_noLighting = noLighting;
}

void
Mesh::setSpecialVCBuffer(Shader::VSCBuffer* vscbuffer)
{
    m_specialVCBuffer = vscbuffer;
}

void
Mesh::setSpecialPCBuffer(Shader::PSCBuffer* pscbuffer)
{
    m_specialPCBuffer = pscbuffer;
}

/* ---------------
   Private Methods
   --------------- */
bool
Mesh::createMesh(vec2f*  optTexCoords,                 
                 uint32  optNTexCoords)
{
    // Disorganized data containers
    std::vector<VertexPos> disorgPos;
    std::vector<VertexTex> disorgTexcoords;
    std::vector<VertexNor> disorgNormals;
    std::vector<ObjIndex>  objIndices;

    // Organized data containers
    std::vector<Vertex> finalVertices;
    std::vector<uint32> finalIndices;
    
    if (isHUDElement() || isBillboard())
    {
        finalVertices.assign(MESH_HUD_VERTICES, MESH_HUD_VERTICES + 4);
        finalIndices.assign(MESH_HUD_INDICES, MESH_HUD_INDICES + 6);
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
    
    if (optTexCoords)
    {
        for (size_t i = 0;
                    i < optNTexCoords; 
                  ++i, ++optTexCoords)
        {
            finalVertices[i].v_tu = optTexCoords->x;
            finalVertices[i].v_tv = optTexCoords->y;
        }
    }

    real32 minWidth  = 0.0f, maxWidth  = 0.0f;
    real32 minDepth  = 0.0f, maxDepth  = 0.0f;
    real32 minHeight = 0.0f, maxHeight = 0.0f;

    // Update Dimensions
    for (auto citer = finalVertices.cbegin();
              citer != finalVertices.cend();
            ++citer)
    {
        if (citer->v_x < minWidth)  minWidth  = citer->v_x;
        if (citer->v_x > maxWidth)  maxWidth  = citer->v_x;
        if (citer->v_y < minHeight) minHeight = citer->v_y;
        if (citer->v_y > maxHeight) maxHeight = citer->v_y;
        if (citer->v_z < minDepth)  minDepth  = citer->v_z;
        if (citer->v_z > maxDepth)  maxDepth  = citer->v_z;
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

/* ------------------
   Internal Functions
   ------------------ */
static void
registerMeshData(const stringID meshID,
                 const Mesh* mesh)
{
    if (isPresent(meshID)) return;
    s_cachedMeshes[meshID] = std::shared_ptr<MeshCachedData>(new MeshCachedData);
    s_cachedMeshes[meshID]->mcd_vertexBuffer = mesh->getVertexBuffer();
    s_cachedMeshes[meshID]->mcd_indexBuffer  = mesh->getIndexBuffer();
    s_cachedMeshes[meshID]->mcd_indexCount   = mesh->getIndexCount();
    s_cachedMeshes[meshID]->mcd_dimensions   = mesh->calculateDimensions();
}

static bool
isPresent(const stringID meshID)
{
    return s_cachedMeshes.find(meshID) !=
           s_cachedMeshes.end();
}

static std::shared_ptr<MeshCachedData>
retrieveMeshData(const stringID meshID)
{
    if (s_cachedMeshes.find(meshID) !=
        s_cachedMeshes.end())
    {
        return s_cachedMeshes[meshID];
    }
    return nullptr;
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
            for (size_t i = 1;
                        i < 4;
                      ++i)
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
    for (size_t i = 0;
                i < objIndices.size();
              ++i)
    {
        Mesh::Vertex finalVertex = {};

        // Position Extraction        
        finalVertex.v_x = disorgPos[objIndices[i].vertexIndex].x;
        finalVertex.v_y = disorgPos[objIndices[i].vertexIndex].y;
        finalVertex.v_z = disorgPos[objIndices[i].vertexIndex].z;

        // Tex Coord Extraction
        finalVertex.v_tu = disorgTexcoords[objIndices[i].texIndex].tu;
        finalVertex.v_tv = disorgTexcoords[objIndices[i].texIndex].tv;

        // Normal Extraction        
        finalVertex.v_nx = disorgNormals[objIndices[i].normIndex].nx;
        finalVertex.v_ny = disorgNormals[objIndices[i].normIndex].ny;
        finalVertex.v_nz = disorgNormals[objIndices[i].normIndex].nz;

        finalVertices.push_back(finalVertex);
        finalIndices.push_back(i);
    }

    return true;
}