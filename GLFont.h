/* Text-rendering library written in C++ for OpenGL 3.3, using FreeType 2. */
/* https://github.com/jtberglund/GLFont */

#pragma once

#include "GL/glew.h"

#include "ft2build.h"
#include FT_FREETYPE_H

class GLFont {
public:
    GLFont(const char* fontFile);
    ~GLFont();

    void setFontFile(const char* fontFile);

    FT_Face getFaceHandle();

private:
    char* _fontFile;
    FT_Error _error;
    FT_Library _ft;
    FT_Face _face;

};
