#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../shader/shader.hpp"

struct Character {
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool init(
        const std::string& fontPath,
        Shader* textShader,
        int screenWidth,
        int screenHeight
    );

    void renderText(
        const std::string& text,
        glm::vec3 color,
        float x,
        float y,
        float scale
    );

private:
    FT_Library ft = nullptr;
    std::map<char, Character> characters;

    unsigned int VAO = 0;
    unsigned int VBO = 0;

    Shader* shader = nullptr;
    int screenW = 0;
    int screenH = 0;
};
