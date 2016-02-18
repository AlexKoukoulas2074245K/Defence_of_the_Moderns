/* ---------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        renderer.cpp

   File description: The implementation of the
   renderer class declared in renderer.h.
   --------------------------------------------- */

#include "renderer.h"
#include "mesh.h"
#include "texture.h"
#include "font.h"
#include "lights.h"
#include "../game/camera.h"
#include "../game/scene.h"
#include "../game/entity.h"
#include "../window.h"
#include "../util/logging.h"
#include "../util/physics.h"

/* -------------
   External Vars
   ------------- */
extern Window* g_window;

/* --------------
   Public Methods
   -------------- */
Renderer*
Renderer::get()
{
    static Renderer instance;
    return &instance;
}

Renderer::~Renderer()
{
    if (m_d3dState)           delete m_d3dState;
    if (m_stdShader)          delete m_stdShader;
    if (m_hudShader)          delete m_hudShader;
    if (m_bilShader)          delete m_bilShader;
    if (m_font)               delete m_font;
    if (m_currentLightBuffer) delete m_currentLightBuffer;
    
    for (auto iter = m_primitiveModels.begin();
              iter != m_primitiveModels.end();
            ++iter) 
    {
        delete iter->second;
    }
}

void
Renderer::beginFrame()
{
    FLOAT clearColor[4] = {1.0f, 0.0f, 0.6f, 1.0f};
    m_d3dState->m_devcon->ClearRenderTargetView(m_d3dState->m_backBuffer.Get(),
                                                clearColor);
    
    m_d3dState->m_devcon->ClearDepthStencilView(m_d3dState->m_depthStencilView.Get(),
                                                D3D11_CLEAR_DEPTH,
                                                1.0f,
                                                0);    
}

void
Renderer::endFrame()
{        
    uint32 syncInterval = m_d3dState->m_vsync ? 1 : 0;
    m_d3dState->m_swapChain->Present(syncInterval, 0);    
}

void
Renderer::renderScene(const Scene* scene)
{   
    // Light accumulation
    m_currentLightBuffer->clear();

    size_t dlIndex = 0U;
    size_t plIndex = 0U;
    
    auto lightVec = scene->getLights();
    for (auto lbegin = lightVec.cbegin();
              lbegin != lightVec.cend();
            ++lbegin)
    {
        switch ((*lbegin)->getType())
        {
            case Light::LIGHT_DIRECTIONAL:
            {
                if (dlIndex >= Shader::SHADER_MAX_DIRECTIONAL_LIGHTS) continue;
                const DirectionalLight* dl = dynamic_cast<const DirectionalLight*>(*lbegin);
                m_currentLightBuffer->pcb_directionalLights[dlIndex].dl_ambientColor = dl->getAmbientColor();
                m_currentLightBuffer->pcb_directionalLights[dlIndex].dl_diffuseColor = dl->getDiffuseColor();
                m_currentLightBuffer->pcb_directionalLights[dlIndex].dl_direction    = dl->getDirection();                
                ++dlIndex;
            } break;

            case Light::LIGHT_POINT:
            {
                if (plIndex >= Shader::SHADER_MAX_POINT_LIGHTS) continue;
                const PointLight* pl = dynamic_cast<const PointLight*>(*lbegin);
                m_currentLightBuffer->pcb_pointLights[plIndex].pl_ambientColor = pl->getAmbientColor();
                m_currentLightBuffer->pcb_pointLights[plIndex].pl_diffuseColor = pl->getDiffuseColor();
                m_currentLightBuffer->pcb_pointLights[plIndex].pl_position     = pl->getPosition();
                m_currentLightBuffer->pcb_pointLights[plIndex].pl_range        = pl->getRange();
                ++plIndex;
            } break;
        }
    }

    // Aggregate mesh list
    std::vector<const Mesh*> meshList;
     
    auto entityVec = scene->getEntities();
    Entity* highlightedEntity = scene->getHighlightedEntity();
    
    std::vector<Entity*> lightingUnnaffected;

    for (auto entityBegin = entityVec.begin();
              entityBegin != entityVec.end();
            ++entityBegin)
    {
        if ((*entityBegin) != highlightedEntity && 
           !(*entityBegin)->isInvisible())        
        {    

            auto bodyVec = (*entityBegin)->getBodies();
            for (auto bodyBegin = bodyVec.begin();
                      bodyBegin != bodyVec.end(); 
                    ++bodyBegin)
            {
                meshList.push_back(*bodyBegin);            
            }
        }
    }
    
    // Normal mesh rendering
    for (auto citer = meshList.cbegin();
              citer != meshList.cend();
            ++citer)
    {       
        renderMesh(*citer,
                   0,
                   (*citer)->getSpecialVCBuffer(),
                   (*citer)->getSpecialPCBuffer());
    }

    // Highlighted mesh rendering 
    if (highlightedEntity)
    {
        auto bodyVec = highlightedEntity->getBodies();
        for (auto bodyBegin = bodyVec.rbegin();
                  bodyBegin != bodyVec.rend();
                ++bodyBegin)
        {
            // Insidious Highlighting hack
            Mesh* mesh = const_cast<Mesh*>(*bodyBegin);
            mesh->scale = vec3f(1.08f, 1.08f, 1.08f);
            m_d3dState->m_devcon->OMSetDepthStencilState(m_d3dState->m_disabledDepth.Get(), 1);
            if(highlightedEntity->isEnemy()) 
            {
                renderMesh(mesh, 2);
            }
            else
            {
                renderMesh(mesh, 1);
            }
            m_d3dState->m_devcon->OMSetDepthStencilState(m_d3dState->m_enabledDepth.Get(), 1);
            m_d3dState->m_devcon->ClearDepthStencilView(m_d3dState->m_depthStencilView.Get(),
                                                        D3D11_CLEAR_DEPTH,
                                                        1.0f,
                                                        0);
            mesh->setHighlighted(false);
            mesh->scale = vec3f(1.0f, 1.0f, 1.0f);
            renderMesh(mesh);
        }
    }
    
    // Finally render healthbars
    for (auto iter = scene->getEnemies().begin();
              iter != scene->getEnemies().end();
            ++iter)
    {
        (*iter)->renderInternalComponents();
    }
}

void
Renderer::renderPrimitive(const Primitive       primitive,
                          const math::Geometry* geometry,
                          const DebugColor      debugColor,
                          const bool            wireframe)
{
    if (wireframe) m_d3dState->m_devcon->RSSetState(m_d3dState->m_wireFrameRastState.Get());
    
    Mesh* mesh = m_primitiveModels[primitive];
    if (!mesh) return;

    mesh->setNoLighting(true);

    cstring texName = nullptr;
    switch (debugColor)
    {
    case DebugColor::CYAN: texName   = "debug_cyan"; break;
    case DebugColor::RED: texName    = "debug_red"; break;
    case DebugColor::YELLOW: texName = "debug_yellow"; break;
    }

    switch (primitive)
    {
        case Primitive::CUBE:
        {
            const math::Cube* cube = dynamic_cast<const math::Cube*>(geometry);
            
            vec3f meshDims         = mesh->calculateDimensions();
            const vec3f& cubeDims  = cube->getDimensions();            
            
            mesh->scale.x *= cubeDims.x / meshDims.x;
            mesh->scale.y *= cubeDims.y / meshDims.y;
            mesh->scale.z *= cubeDims.z / meshDims.z;

            mesh->position = cube->getPosition();

        }break;

        case Primitive::PLANE:
        {
            const math::GeoPlane* plane = dynamic_cast<const math::GeoPlane*>(geometry);

            vec3f meshDims         = mesh->calculateDimensions();
            const vec2f& planeDims = plane->getDimensions();

            mesh->scale.x *= planeDims.x / meshDims.x;
            mesh->scale.y  = 1.0f;
            mesh->scale.z *= planeDims.y / meshDims.z;

            mesh->position = plane->getPosition();

        }break;

        case Primitive::SPHERE:
        {
            const math::Sphere* sphere = dynamic_cast<const math::Sphere*>(geometry);
            
            // Calculate new primitive dimensions
            vec3f currMeshDims    = mesh->calculateDimensions();
            real32 sphereDiameter = sphere->getRadius() * 2;

            mesh->scale.x *= sphereDiameter / currMeshDims.x;
            mesh->scale.y *= sphereDiameter / currMeshDims.y;
            mesh->scale.z *= sphereDiameter / currMeshDims.z;

            // Set primitive position
            mesh->position = sphere->getPosition();            

        }break;
    }
    
    mesh->loadNewTexture(texName);
    renderMesh(mesh);
    
    if (wireframe) m_d3dState->m_devcon->RSSetState(m_d3dState->m_solidRastState.Get());
}

void
Renderer::renderString(const cstring str,
                       const real32  x,
                       const real32  y)
{
    real32 xCounter = x;
    for (size_t i = 0;
                i < strlen(str);
              ++i, xCounter += m_font->getSize() / 1.5f)
    {
        uint8 currChar = str[i];
        if (currChar == ' ') continue;

        std::shared_ptr<Mesh> currGlyph = m_font->getGlyph(currChar);
        
        currGlyph->position.x = xCounter;
        currGlyph->position.y = y;
        renderMesh(currGlyph.get());        
    }
}

void
Renderer::renderMesh(const Mesh* mesh,
                     const int highlightOption /* 0 */,
                     Shader::VSCBuffer* externVertexBuffer /* nullptr */,
                     Shader::PSCBuffer* externPixelBuffer /* nullptr */)
{
    if (!mesh) return;
    if (!mesh->isHUDElement() && !testVisible(mesh)) return;    
    if (!m_currentCam)
    {
        logline("Camera has not been set");
        return;
    }
        
    uint32 stride = sizeof(Mesh::Vertex);
    uint32 offset = 0U;

    mat4x4 projMatrix = m_currentCam->calculateProjectionMatrix();
    mat4x4 viewMatrix = m_currentCam->calculateViewMatrix();    
    mat4x4 worldMat = mesh->getWorldMatrix();    
    mat4x4 finalMat = worldMat * viewMatrix * projMatrix;  
    mat4x4 rotMat = mesh->getRotationMatrix();

    Shader::VSCBuffer vcbuffer = {};
    vcbuffer.vcb_mvpMatrix         = finalMat;
    vcbuffer.vcb_rotationMatrix    = rotMat;
    vcbuffer.vcb_worldMatrix       = worldMat;
    vcbuffer.vcb_eyePosition       = math::getVec4f(m_currentCam->getPosition());
    vcbuffer.vcb_highlight         = highlightOption;    

    Shader* currentShader = nullptr;
    if (mesh->isHUDElement())   currentShader   = m_hudShader;
    else if (mesh->isBillboard() ||
             mesh->noLighting()) currentShader  = m_bilShader;
    else currentShader                          = m_stdShader;
        
    Shader::VSCBuffer* finalVCBuffer = externVertexBuffer ? externVertexBuffer : &vcbuffer;
    Shader::PSCBuffer* finalPCBuffer = externPixelBuffer  ? externPixelBuffer  : m_currentLightBuffer;

    m_d3dState->m_devcon->UpdateSubresource(currentShader->getVSCBuffer().Get(), NULL, NULL, finalVCBuffer, NULL, NULL);
    m_d3dState->m_devcon->UpdateSubresource(currentShader->getPSCBuffer().Get(), NULL, NULL, finalPCBuffer, NULL, NULL);

    m_d3dState->m_devcon->IASetVertexBuffers(0, 1, mesh->getVertexBuffer().GetAddressOf(), &stride, &offset);
    m_d3dState->m_devcon->IASetIndexBuffer(mesh->getIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0U);
    m_d3dState->m_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    m_d3dState->m_devcon->VSSetShader(currentShader->getVertexShader().Get(), NULL, NULL);
    m_d3dState->m_devcon->VSSetConstantBuffers(0, 1, currentShader->getVSCBuffer().GetAddressOf());

    m_d3dState->m_devcon->PSSetShader(currentShader->getPixelShader().Get(), NULL, NULL);
    m_d3dState->m_devcon->PSSetConstantBuffers(0, 1, currentShader->getPSCBuffer().GetAddressOf());
    m_d3dState->m_devcon->PSSetShaderResources(0, 1, mesh->getTexture()->getTexturePointer().GetAddressOf());
    
    if  (mesh->isHUDElement()) m_d3dState->m_devcon->OMSetDepthStencilState(m_d3dState->m_disabledDepth.Get(), 1);
    if (!mesh->isHUDElement()) m_d3dState->m_devcon->OMSetDepthStencilState(m_d3dState->m_enabledDepth.Get(), 1);
    
    m_d3dState->m_devcon->DrawIndexed(mesh->getIndexCount(), 0, 0);    
}

void
Renderer::setCamera(const Camera* camera)
{
    m_currentCam = camera;
}

comptr<ID3D11Device>
Renderer::getDeviceHandle() bitwise_const
{
    return m_d3dState->m_device;
}

comptr<ID3D11DeviceContext>
Renderer::getDevconHandle() bitwise_const
{
    return m_d3dState->m_devcon;
}

/* ---------------
   Private Methods
   --------------- */
Renderer::Renderer():

    m_d3dState(new D3D11State()),        
    m_stdShader(new Shader("std")),
    m_hudShader(new Shader("hud")),
    m_bilShader(new Shader("bil")),
    m_font(new Font("font_1", 0.1f)),
    m_currentLightBuffer(new Shader::PSCBuffer)
{   
    // Preload debug textures
    Texture red("debug_red");
    Texture cyan("debug_cyan");
    Texture yellow("debug_yellow");

    m_primitiveModels[Primitive::CUBE]   = new Mesh("sample_cube",   Mesh::MESH_TYPE_NORMAL);
    m_primitiveModels[Primitive::PLANE]  = new Mesh("sample_plane",  Mesh::MESH_TYPE_NORMAL);
    m_primitiveModels[Primitive::SPHERE] = new Mesh("sample_sphere", Mesh::MESH_TYPE_NORMAL);
}

bool
Renderer::testVisible(const Mesh* mesh)
{
    math::Frustum camFrustum;
    m_currentCam->calculateFrustum(&camFrustum);
    
    return !physics::intersectionTest(physics::PHYSICS_INTERSECTION_TYPE_FRUSTUMSPHERE,
                                      &camFrustum,
                                      &mesh->getVisibleGeometry());
}

