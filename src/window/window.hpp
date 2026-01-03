#pragma once

#include <memory>
#include <string>
#include <GLFW/glfw3.h>

#include "input/inputManager.hpp"
#include "../misc/stateManager.hpp"
#include "../render/loadingScreenRenderer.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Forward declaration
class SceneRenderer;



/*
 Class: Window
 ----------------
 Handles creation and management of the GLFW window.
 Manages OpenGL initialization, input processing, rendering loop, and ImGui.
*/
class Window
{
private:

    // for window size type
    struct WindowSize {
        unsigned int x;
        unsigned int y;
    };
    // Window size
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

public:
    Window(StateManager::GameStateManager& _gameStateManager);
    ~Window() = default;
    void initialize(float camX, float camY, float camZ);
    //void initSceneRenderer();
    void createWindow();
    void loadOpenGL();
    void mainLoop();
    void terminateWindow();

    WindowSize getWindowSize() {
		return { SCR_WIDTH, SCR_HEIGHT };
    }

    void swapBuffers() {
        glfwSwapBuffers(window.get());
	}
    void pollEvents() {
        glfwPollEvents();
    }

    void clearColor();

    GLFWwindow* getGLFWwindow() {
        return window.get();
    }

	// Set the loading screen renderer pointer for later use.
	// Must be set before main loop starts.
    void setLoadingRenderer(LoadingScreenRenderer* _renderer);

private:

    void processInput(World* world);
	void initImGui();
	void renderImGui();
	void startImGuiFrame();
	void terminateImGui();

    

    // ---------------------------------------------------
    // Members
    // ---------------------------------------------------
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window{ nullptr, glfwDestroyWindow };
	LoadingScreenRenderer* loadingRenderer = nullptr;
    std::unique_ptr<SceneRenderer> sceneRenderer;
    std::unique_ptr<InputManager> inputManager;


    

    // make sure to only have one instance of GameStateManager, here we have a referance to it.
    // the ref points to the StateManager in game3D.cpp
    StateManager::GameStateManager& gameState;

    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;



    // Text & overlays
    std::string fontFile = "fonts/arial.ttf";
    std::string pausedText = "Paused";
    std::string interactText = "Interact using E";


};
