#pragma once

#include "../include/shader.hpp"


class TextureLoader
{
public:
	unsigned int texture1, texture2;
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* texData;
	void loadTextures(Shader* _ourShader);
};

