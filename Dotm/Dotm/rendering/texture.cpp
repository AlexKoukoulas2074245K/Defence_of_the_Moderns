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
#include <map>

/* -------------
   Internal Vars
   ------------- */
std::map<stringID, comptr<ID3D11ShaderResourceView>> s_cachedTextures;

/* -------------------
   Internal Signatures
   ------------------- */
void
registerTexture(const stringID textureID,
                const Texture* texture);

bool
isPresent(const stringID textureID);

comptr<ID3D11ShaderResourceView>
retrieveTexture(const stringID textureID);
   
/* --------------
   Public Methods
   -------------- */

Texture::Texture(cstring textureName):

                 m_name(internString(textureName))
{
    if (isPresent(m_name))
    {
        m_texture = retrieveTexture(m_name);
        return;
    }

    std::wstring widePath = L"assets/textures/" + 
                            string_utils::getwstring(retrieveString(m_name)) +
                            L".png";

    HRESULT result = DirectX::CreateWICTextureFromFile(Renderer::get()->getDeviceHandle().Get(),
                                                       Renderer::get()->getDevconHandle().Get(),
                                                       widePath.c_str(),
                                                       NULL,
                                                       &m_texture,
                                                       0);
    
    if (FAILED(result))
    {        
        DirectX::CreateWICTextureFromFile(Renderer::get()->getDeviceHandle().Get(),
                                          Renderer::get()->getDevconHandle().Get(),
                                          L"assets/textures/missing.png",
                                          NULL,
                                          &m_texture,
                                          0);
        
        MessageBoxW(NULL,
                    (L"Missing texture: " + widePath).c_str(),
                    L"Missing Texture!",
                    MB_ICONEXCLAMATION);
        
        return;
    }

    registerTexture(m_name, this);
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


/* ------------------
   Internal Functions
   ------------------ */
void
registerTexture(const stringID textureID, 
                const Texture* texture)
{
    if (isPresent(textureID)) return;
    s_cachedTextures[textureID] = texture->getTexturePointer();
}

bool
isPresent(const stringID textureID)
{
    return s_cachedTextures.find(textureID) != 
           s_cachedTextures.end();
}

comptr<ID3D11ShaderResourceView>
retrieveTexture(const stringID textureID)
{
    if (isPresent(textureID)) return s_cachedTextures[textureID];
    return nullptr;
}