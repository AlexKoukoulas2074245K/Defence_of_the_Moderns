/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        d3d11state.h
   
   File description: A class encapsulating 
   the initialization, destruction and offering
   mutators for the various direct3d11 com objects
   utilized across the program
   ----------------------------------------------- */

#pragma once

#include "d3d11common.h"
#include "../util/strings.h"
#include "../dotmdef.h"

class D3D11State
{
public:

    D3D11State();

    ~D3D11State();

    D3D11State(const D3D11State& rhs) = delete;

    D3D11State&
    operator = (const D3D11State& rhs) = delete;

    bool
    isVsyncEnabled() logical_const;

    bool
    isMultisamplingEnabled() logical_const;

private:

    void
    initConfig();

    void
    initD3D();

private:

    comptr<ID3D11Device>            m_device;
    comptr<ID3D11DeviceContext>     m_devcon;
    comptr<IDXGISwapChain>          m_swapChain;
    comptr<ID3D11RenderTargetView>  m_backBuffer;
    comptr<ID3D11Texture2D>         m_depthBuffer;
    comptr<ID3D11DepthStencilState> m_enabledDepth;
    comptr<ID3D11DepthStencilState> m_disabledDepth;
    comptr<ID3D11DepthStencilView>  m_depthStencilView;
    comptr<ID3D11BlendState>        m_blendState;
    comptr<ID3D11SamplerState>      m_samplerState;
    comptr<ID3D11RasterizerState>   m_solidRastState;
    comptr<ID3D11RasterizerState>   m_wireFrameRastState;
    
    bool   m_vsync;
    bool   m_multisampling;
    bool   m_renderSolid;
    uint32 m_anisotropic;
    
    stringID m_videoCardName;
    uint32   m_videoCardMemory;
};