#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "../include/shaderProgram.hpp"
#include "shader.hpp"
#include "textureLoader.hpp"
#include "../include/world.hpp"
#include "../include/textRenderer.hpp"

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

	bool paused = false;
	bool escPrevPressed = false;
	bool spacePrevPressed = false;
	bool toggleGravityPressed = false;
	bool spawnInteractablePressed = false;
	bool interactPressed = false;


	GLFWwindow* window;
	Shader* sceneShader;
	Shader* textShader;

	TextRenderer textRenderer;

	TextureLoader textureLoader;

	// functions
	// Must call these functions in this spacific order.
	// ---------------------------------------------------

	void initialize(float _camX, float _camY, float _camZ);
	void createWindow();
	void loadOpenGL();
	void createShaderProgram();
	void loadTextures();
	void mainLoop(World* _world);
	void terminateWindow();

	// ---------------------------------------------------

private:

	void processInput(GLFWwindow* window, World* _world);

	/*
	Renders the 3D scene
	*/
	void render();

};


