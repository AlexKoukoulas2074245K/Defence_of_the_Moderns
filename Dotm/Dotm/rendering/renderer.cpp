/* ---------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        renderer.cpp

   File description: The implementation of the
   renderer class declared in renderer.h.
   --------------------------------------------- */

#include "renderer.h"
#include "d3d11state.h"

/* --------------
   Public Methods
   -------------- */
Renderer&
Renderer::get()
{
    static Renderer instance;
    return instance;
}

/* ---------------
   Private Methods
   --------------- */
Renderer::Renderer():

    m_d3dState(new D3D11State())
{
  
}

Renderer::~Renderer()
{
    
}