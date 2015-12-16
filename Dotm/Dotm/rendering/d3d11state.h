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
#include "../strings.h"
#include "../dotmdef.h"

class D3D11State
{
public:

    D3D11State();

    ~D3D11State();

    D3D11State(const D3D11State& rhs) = delete;

    D3D11State&
    operator = (const D3D11State& rhs) = delete;

private:

    void
    init();

private:

    comptr<ID3D11Device>           m_device;
    comptr<ID3D11DeviceContext>    m_devcon;
    comptr<IDXGISwapChain>         m_swapChain;
    comptr<ID3D11RenderTargetView> m_backBuffer;
    comptr<ID3D11Texture2D>        m_depthBuffer;

    stringID m_videoCardName;
    uint32   m_videoCardMemory;
};