/* ----------------------------------------------
   Author:           Alex Koukoulas
   Date:             28/12/2015
   File name:        texture.h
   
   File description: A simple class encapsulating 
   a ID3D11ShaderResourceView (texture) to
   be used on meshes
   ---------------------------------------------- */

#pragma once

#include "d3d11common.h"
#include "../util/strings.h"
#include "../dotmdef.h"

class Texture
{

public:

    Texture(cstring textureName);

    ~Texture();
    
    comptr<ID3D11ShaderResourceView>
    getTexturePointer() bitwise_const;

    cstring
    getTextureName() logical_const;

private:

    comptr<ID3D11ShaderResourceView> m_texture;
    stringID                         m_name;

};
