/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        d3d11state.cpp
   
   File description: The implementation of the
   class D3D11State declared in d3d11state.h
   ----------------------------------------------- */

#include "d3d11state.h"
#include "../window.h"
#include "../config/configparser.h"

/* ----------------------
   Preprocessor Constants
   ---------------------- */
#define FLOAT_MAX 3.402823466e+38F

/* --------------
   External Vars
   -------------- */
extern Window* g_window;

/* --------------
   Public Methods
   -------------- */
D3D11State::D3D11State():

    m_backBuffer(nullptr),
    m_depthBuffer(nullptr),
    m_devcon(nullptr),
    m_device(nullptr),
    m_swapChain(nullptr)
{
    initConfig();
    initD3D();    
}

D3D11State::~D3D11State()
{

}

/* ---------------
   Private Methods
   --------------- */
void
D3D11State::initConfig()
{
    // Assert that the rendering config file exists
    if (!config::initConfigFile("rendconfig")) return;

    // Extract the rendering config variables from the config file
    config::extractConfigBool("rendconfig", "vsync", &m_vsync);
    config::extractConfigBool("rendconfig", "multisampling", &m_multisampling);
    config::extractConfigUint("rendconfig", "anisotropic", &m_anisotropic);
}

void
D3D11State::initD3D()
{
    // DXGI Factory Creation
    comptr<IDXGIFactory> factory;
    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**) &factory);

    // Create Graphics Card Adapter
    comptr<IDXGIAdapter> adapter;
    HR(factory->EnumAdapters(0, &adapter));

    // Enum the primary adapter output
    comptr<IDXGIOutput> adapterOutput;
    HR(adapter->EnumOutputs(0, &adapterOutput));

    // Get number of modes that fir the unorm display format 
    uint32 numModes;
    HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                         DXGI_ENUM_MODES_INTERLACED,
                                         &numModes,
                                         nullptr));

    // Fill Display Modes
    DXGI_MODE_DESC* displayModes = new DXGI_MODE_DESC[numModes];
    HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                         DXGI_ENUM_MODES_INTERLACED,
                                         &numModes,
                                         displayModes));

    uint32 refRateNum   = 0U;
    uint32 refRateDenum = 0U;

    // Find the appropriate display mode for the current window
    // resolution
    for (size_t i = 0;
                i < numModes;
              ++i)
    {
        if (displayModes[i].Width  == (uint32) g_window->getWidth() &&
            displayModes[i].Height == (uint32) g_window->getHeight())
        {
            refRateNum   = displayModes[i].RefreshRate.Numerator;
            refRateDenum = displayModes[i].RefreshRate.Denominator;
            break;
        }
    }

    delete[] displayModes;

    // Get Video Card Description
    DXGI_ADAPTER_DESC adapterDesc = {};
    HR(adapter->GetDesc(&adapterDesc));

    // Store Video Card and System Details
    m_videoCardName   = internString(adapterDesc.Description);
    m_videoCardMemory = (uint32) (adapterDesc.DedicatedVideoMemory / 1024 / 1024);
    stringID cardMem  = internString((void*) m_videoCardMemory);

    // Print out the video card details
    OutputDebugString(retrieveString(m_videoCardName));
    OutputDebugString("\n");
    OutputDebugString(retrieveString(cardMem));
    OutputDebugString("\n");

    // Fill out the Swap Chain Description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width                   = g_window->getWidth();
	swapChainDesc.BufferDesc.Height                  = g_window->getHeight();
    swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator   = m_vsync ? refRateNum : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = m_multisampling ? refRateDenum : 1;
	swapChainDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    
    uint32 msValue = m_multisampling ? 4U : 1U;
    swapChainDesc.SampleDesc = {msValue, 0};

    swapChainDesc.BufferCount   = 1;
    swapChainDesc.BufferUsage   = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow  = g_window->getHandle();
    swapChainDesc.Windowed      = !g_window->isFullscreen();
    swapChainDesc.SwapEffect    = DXGI_SWAP_EFFECT_DISCARD;
    D3D_FEATURE_LEVEL featLevel = D3D_FEATURE_LEVEL_11_0;

    // Create Device, Device context and Swap Chain
    HR(D3D11CreateDeviceAndSwapChain(nullptr,
                                    D3D_DRIVER_TYPE_HARDWARE,
                                    nullptr,
                                    0,
                                    &featLevel,
                                    1,
                                    D3D11_SDK_VERSION,
                                    &swapChainDesc,
                                    &m_swapChain,
                                    &m_device,
                                    nullptr,
                                    &m_devcon));

    // Disable alt-enter
    IDXGIFactory1 *pFactory = NULL;

    if (SUCCEEDED(m_swapChain->GetParent(__uuidof (IDXGIFactory1), (void **) &pFactory)))
    {
        pFactory->MakeWindowAssociation(g_window->getHandle(), DXGI_MWA_NO_ALT_ENTER);
        pFactory->Release();
    }

    // Create the render target view
    comptr<ID3D11Texture2D> backBuffer;
    HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backBuffer));
    HR(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_backBuffer));
    
    // Create Depth Buffer
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    depthBufferDesc.Width      = g_window->getWidth();
    depthBufferDesc.Height     = g_window->getHeight();
    depthBufferDesc.MipLevels  = 1;
    depthBufferDesc.ArraySize  = 1;
    depthBufferDesc.Format     = DXGI_FORMAT_D24_UNORM_S8_UINT;    
    depthBufferDesc.SampleDesc = {msValue, 0};
    depthBufferDesc.BindFlags  = D3D11_BIND_DEPTH_STENCIL;

    // Create the depth buffer
    HR(m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthBuffer));

    // Create custom depth stencil states
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable      = true;
    depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable    = true;
    depthStencilDesc.StencilReadMask  = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

    // Create the two depth stencil states
    HR(m_device->CreateDepthStencilState(&depthStencilDesc, &m_enabledDepth));
    depthStencilDesc.DepthEnable = false;
    HR(m_device->CreateDepthStencilState(&depthStencilDesc, &m_disabledDepth));

    // Set the depth stencil state to have depth enabled initially
    m_devcon->OMSetDepthStencilState(m_enabledDepth.Get(), 1);

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    
    if  (m_multisampling) depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    if (!m_multisampling) depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    HR(m_device->CreateDepthStencilView(m_depthBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView));
    
    // Set render target and depth stencil view
    m_devcon->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthStencilView.Get());

    // Create the blend description
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable           = TRUE;
    blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.AlphaToCoverageEnable  = TRUE;

    // Create and set the blend state
    HR(m_device->CreateBlendState(&blendDesc, &m_blendState));
    m_devcon->OMSetBlendState(m_blendState.Get(), 0, 0xFFFFFFFF);

    // Create the sampler description
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter         = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy  = m_anisotropic;
    samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.MinLOD         = 0.0f;
    samplerDesc.MaxLOD         = FLOAT_MAX;
    samplerDesc.MipLODBias     = 0.0f;

    // Create and set the sampler
    HR(m_device->CreateSamplerState(&samplerDesc, &m_samplerState));    

    m_devcon->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    // Create the custom rasterizer description
    D3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.FillMode              = D3D11_FILL_SOLID;
    rastDesc.CullMode              = D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = FALSE;
    rastDesc.DepthClipEnable       = TRUE;
    rastDesc.ScissorEnable         = FALSE;
    rastDesc.AntialiasedLineEnable = TRUE;
    rastDesc.MultisampleEnable     = TRUE;
    rastDesc.DepthBias             = 0;
    rastDesc.DepthBiasClamp        = 0.0f;
    rastDesc.SlopeScaledDepthBias  = 0.0f;

    // Create the two rasterizer states
    HR(m_device->CreateRasterizerState(&rastDesc, &m_solidRastState));
    rastDesc.FillMode = D3D11_FILL_WIREFRAME;
    HR(m_device->CreateRasterizerState(&rastDesc, &m_wireFrameRastState));
    
    // Set the solid rasterizer state as the initial 
    m_devcon->RSSetState(m_solidRastState.Get());

    // Create viewport description
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width    = (real32) g_window->getWidth();
    viewport.Height   = (real32) g_window->getHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    
    // Set the viewport
    m_devcon->RSSetViewports(1, &viewport);
}