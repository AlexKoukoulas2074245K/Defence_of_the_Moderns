/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             28/12/2015
   File name:        font.h
   
   File description: A class encapsulating 
   the properties of a font family ready 
   for rendering
   ------------------------------------------ */

#pragma once

#include "../util/strings.h"
#include "../dotmdef.h"
#include <memory>
#include <unordered_map>

class Texture;
class Mesh;
class Font
{

public:

    // Font size in normalized screen coords here.
    // a medium font size would be 0.1f
    Font(cstring fontName,
         real32  fontSize);

    ~Font();

    std::shared_ptr<Mesh>
    getGlyph(uint8 chr);

    void
    setSize(const real32 size);

    real32
    getSize() logical_const;

private:

    void
    loadTexture();
    
    void
    loadGlyphs();

private:

    stringID                                         m_fontName;
    std::shared_ptr<Texture>                         m_fontTexture;
    std::unordered_map<uint8, std::shared_ptr<Mesh>> m_glyphs;
    real32                                           m_fontSize;

};
