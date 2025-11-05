#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "../include/shaderProgram.hpp"
#include "shader.hpp"
#include "textureLoader.hpp"
#include "../include/world.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


/*
 Class: Window
 ----------------
 This class handles creation and management of the application window using GLFW.
 It includes methods for initializing GLFW, creating the window, loading OpenGL functions,
 running the main rendering loop, processing input, and terminating the window.

*/
class Window
{
public:
	// settings

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow* window;
	//ShaderProgram* shaderProgram;
	Shader* ourShader;
	TextureLoader textureLoader;

	// functions
	// Must call these functions in this spacific order.
	// ---------------------------------------------------

	void initialize(float _camX, float _camY, float _camZ);
	void createWindow();
	void loadOpenGL();
	void createShaderProgram();
	
	void mainLoop(World* _world);
	void terminateWindow();

	// ---------------------------------------------------

private:

	void processInput(GLFWwindow* window);
	void render();

};


