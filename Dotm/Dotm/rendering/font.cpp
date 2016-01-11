/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             28/12/2015
   File name:        font.cpp
   
   File description: An implementation of the
   font class declared in font.h
   ------------------------------------------ */

#include "font.h"
#include "texture.h"
#include "mesh.h"
#include "../util/stringutils.h"
#include <fstream>
#include <string>

/* --------------
   Public Methods
   -------------- */
Font::Font(cstring fontName,
           real32  fontSize):

           m_fontName(internString(fontName)),
           m_fontSize(fontSize)
{
    loadTexture();
    loadGlyphs();
}

Font::~Font()
{    
}

std::shared_ptr<Mesh>
Font::getGlyph(uint8 chr) 
{
    if(m_glyphs.count(chr)) return m_glyphs[chr];
    return nullptr;
}

void
Font::setSize(const real32 size)
{
    m_fontSize = size;
    for (auto iter = m_glyphs.begin();
         iter != m_glyphs.end();
         ++iter)
    {
        iter->second->scale = vec3f(m_fontSize,
                                    m_fontSize,
                                    m_fontSize);          
    }
}

real32
Font::getSize() logical_const
{
    return m_fontSize;
}

/* ---------------
   Private Methods
   --------------- */

void
Font::loadTexture()
{    
    m_fontTexture.reset(new Texture(retrieveString(m_fontName)));
}

void
Font::loadGlyphs()
{
    std::string fontLayoutPath = retrieveString(m_fontName);
    fontLayoutPath = "assets/fonts/" + fontLayoutPath + ".fnt";
    std::ifstream fontFile(fontLayoutPath.c_str());

    if (!fontFile.is_open())
    {
        MessageBox(NULL,
                   ("Missing font layout file: " + fontLayoutPath).c_str(),
                   "Font Layout Missing!",
                   MB_ICONERROR);
        return;
    }

    std::string line;
    
    // Get expected textures dimensions
    std::getline(fontFile, line);
    std::vector<std::string> texDims;
    string_utils::split(line, ' ', texDims);
    uint32 texWidth  = std::stoi(texDims[0]);
    uint32 texHeight = std::stoi(texDims[1]);

    // Get the glyph box size
    std::getline(fontFile, line);
    uint32 boxSize = std::stoi(line);
    
    // Begin core parsing of the file
    std::vector<std::string> glyphChars;
    real32 xCoord, yCoord;
    xCoord = yCoord = 0U;

    for (; std::getline(fontFile, line);)
    {        
        // Grab Glyph line
        glyphChars.clear();
        string_utils::split(line, ' ', glyphChars);
        
        // Iterate through the glyphs and create
        // the respective mesh
        for (auto citer = glyphChars.cbegin();
             citer != glyphChars.cend();
             ++citer)
        {
            uint8 currChar = citer->at(0);
            
            // Tex coord calculation
            vec2f customTexCoords[] = { vec2f( xCoord / texWidth,            yCoord / texHeight),
                                        vec2f((xCoord + boxSize) / texWidth, yCoord / texHeight),
                                        vec2f((xCoord + boxSize) / texWidth, (yCoord + boxSize) / texHeight),
                                        vec2f( xCoord / texWidth,            (yCoord + boxSize) / texHeight) };            
            
            std::shared_ptr<Mesh> glyph(new Mesh("sample_plane",
                                                 Mesh::MESH_EXTERNAL_TEXCOORDS | Mesh::MESH_TYPE_HUD,
                                                 customTexCoords,
                                                 ARRAYSIZE(customTexCoords)));
                                    
            glyph->setTexture(m_fontTexture);            
            m_glyphs[currChar] = glyph;
            xCoord += boxSize;
        }
        xCoord  = 0U;
        yCoord += boxSize;
    }

    setSize(m_fontSize);
}