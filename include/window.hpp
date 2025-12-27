#pragma once

#include "world.hpp"
#include "shader.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

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
	bool openPlayerInvPressed = false;


	//GLFWwindow* window;
	//Shader* sceneShader;
	//Shader* textShader;

	//TextRenderer textRenderer;

	//TextureLoader textureLoader;

	// functions
	// Must call these functions in this spacific order.
	// ---------------------------------------------------

	void initialize(float _camX, float _camY, float _camZ);
	void imGuiNewFrame();
	void createWindow();
	void loadOpenGL();
	void createShaderProgram();
	void loadTextures();
	void mainLoop(World* _world);
	void terminateWindow();
	Shader* getSceneShader();
	Shader* getTextShader();


	// ---------------------------------------------------

private:

	void processInput(GLFWwindow*& window, World*& _world);

	/*
	Renders the 3D scene
	*/
	void renderScene(World*& _world, Shader* _shader);
	void renderTextOverlays(World*& _world);
	void renderImGuiOverlay(World*& _world);



	void cleanupImGui();

};


