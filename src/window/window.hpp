#pragma once

#include <memory>
#include <string>
#include <GLFW/glfw3.h>

#include "../world/world.hpp"
#include "../shader/shader.hpp"
#include "input/inputManager.hpp"

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
public:
    // ---------------------------------------------------
    // Public API — call in this order
    // ---------------------------------------------------
    void initialize(float camX, float camY, float camZ);
    //void initSceneRenderer();
    void createWindow();
    void loadOpenGL();
    void createShaderProgram(); // deprecated
    void loadTextures();        // deprecated
    void imGuiNewFrame();
    void mainLoop(World* world);
    void terminateWindow();

private:
    // ---------------------------------------------------
    // Input and rendering
    // ---------------------------------------------------
    void processInput(World* world);
    void cleanupImGui();

    // ---------------------------------------------------
    // Members
    // ---------------------------------------------------
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> window{ nullptr, glfwDestroyWindow };
    std::unique_ptr<SceneRenderer> sceneRenderer;
    std::unique_ptr<InputManager> inputManager;

    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Window size
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    // Text & overlays
    std::string fontFile = "fonts/arial.ttf";
    std::string pausedText = "Paused";
    std::string interactText = "Interact using E";
};
