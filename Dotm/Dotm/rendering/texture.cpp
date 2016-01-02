/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             28/12/2015
   File name:        texture.cpp
   
   File description: The implementation of the
   Texture class declared in texture.h
   ---------------------------------------------- */

#include "texture.h"
#include "ddsloader.h"
#include "../util/stringutils.h"
#include "renderer.h"
#include <string>

Texture::Texture(cstring textureName):

    m_name(internString(textureName))
{
    std::wstring widePath = L"assets/textures/" + 
                            string_utils::getwstring(retrieveString(m_name)) +
                            L".png";

    HRESULT result = DirectX::CreateWICTextureFromFile(Renderer::get().getDeviceHandle().Get(),
                                                       Renderer::get().getDevconHandle().Get(),
                                                       widePath.c_str(),
                                                       NULL,
                                                       &m_texture,
                                                       0);
    
    if (FAILED(result))
    {        
        DirectX::CreateWICTextureFromFile(Renderer::get().getDeviceHandle().Get(),
                                          Renderer::get().getDevconHandle().Get(),
                                          L"assets/textures/missing.png",
                                          NULL,
                                          &m_texture,
                                          0);
        
        MessageBoxW(NULL,
                    (L"Missing texture: " + widePath).c_str(),
                    L"Missing Texture!",
                    MB_ICONEXCLAMATION);
    }
}

Texture::~Texture()
{

}

comptr<ID3D11ShaderResourceView>
Texture::getTexturePointer() bitwise_const
{
    return m_texture;
}

cstring
Texture::getTextureName() logical_const
{
    return retrieveString(m_name);
}