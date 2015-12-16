/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        d3d11state.cpp
   
   File description: The implementation of the
   class D3D11State declared in d3d11state.h
   ----------------------------------------------- */

#include "d3d11state.h"
#include "../window.h"

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
    init();
}

D3D11State::~D3D11State()
{

}

/* ---------------
   Private Methods
   --------------- */
void
D3D11State::init()
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
                                         NULL));

    // Fill Display Modes
    DXGI_MODE_DESC* displayModes = new DXGI_MODE_DESC[numModes];
    HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                         DXGI_ENUM_MODES_INTERLACED,
                                         &numModes,
                                         displayModes));

    uint32 refRateNum, refRateDenum;

    // Find the appropriate display mode for the current window
    // resolution
    for (size_t i = 0; i < numModes; ++i)
    {
        if (displayModes[i].Width  == static_cast<uint32>(g_window->getWidth()) &&
            displayModes[i].Height == static_cast<uint32>(g_window->getHeight()))
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
    m_videoCardMemory = static_cast<uint32>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
    stringID cardMem  = internString((void*) m_videoCardMemory);

    OutputDebugString(retrieveString(m_videoCardName));
    OutputDebugString("\n");
    OutputDebugString(retrieveString(cardMem));
    OutputDebugString("\n");
}