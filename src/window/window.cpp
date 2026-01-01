#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Window.hpp"
#include "input/inputManager.hpp"
#include "../render/sceneRenderer.hpp"
#include "../misc/programLogger.hpp"
//#include "../misc/stb_image.h"

using ProgramLogger::log;
using ProgramLogger::LogLevel;

// --------------------------------------------------------
// GLFW Callbacks
// --------------------------------------------------------
namespace {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (!window) return;

        // Retrieve InputManager from window user pointer
        auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (inputManager)
            inputManager->processMouseMovement(xpos, ypos);
    }

    void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset)
    {
        if (!window) return;

        auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (inputManager)
            inputManager->processMouseScroll(yoffset);
    }

} // namespace

// --------------------------------------------------------
// Window Methods
// --------------------------------------------------------
void Window::initialize(float _camX, float _camY, float _camZ)
{
    log("Window initializer", LogLevel::DEBUG);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //stbi_set_flip_vertically_on_load(true);

}

//void Window::initSceneRenderer()
//{
//    // Initialize scene renderer
//    sceneRenderer = std::make_unique<SceneRenderer>(SCR_WIDTH, SCR_HEIGHT);
//
//}

void Window::createWindow()
{
    // 1. Create GLFW window
    GLFWwindow* win = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!win)
    {
        log("Failed to create GLFW Window", LogLevel::ERROR);
        glfwTerminate();
        return;
    }

    window.reset(win);
    glfwMakeContextCurrent(window.get());

	// now that contexts exist, we can load OpenGL functions
	loadOpenGL();

    // 2. Initialize sceneRenderer first
    sceneRenderer = std::make_unique<SceneRenderer>(SCR_WIDTH, SCR_HEIGHT);

    // 3. Initialize InputManager
    inputManager = std::make_unique<InputManager>(window.get(), *sceneRenderer);

    // 4. Attach InputManager pointer to window user pointer for callbacks
    glfwSetWindowUserPointer(window.get(), inputManager.get());

    // 5. Set GLFW callbacks (now safe, because objects exist)
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.get(), mouse_callback);
    glfwSetScrollCallback(window.get(), scroll_callback);

    // 6. Set cursor input mode
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    log("Window created and callbacks set", LogLevel::DEBUG);
}


void Window::loadOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log("Failed to initialize GLAD", LogLevel::ERROR);
        return;
    }
    log("OpenGL functions have been loaded successfuly", LogLevel::DEBUG);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void Window::imGuiNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::createShaderProgram()
{
    log("CreateShaderProgram is no longer needed", LogLevel::WARNING);
}

void Window::loadTextures()
{
    log("loadTextures is no longer needed", LogLevel::WARNING);
}

void Window::mainLoop(World* _world)
{
    if (!_world)
    {
        log("World pointer is null in Window::mainLoop", LogLevel::ERROR);
        //return;
    }

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window.get()))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(_world);

        sceneRenderer->RenderScene();

        glfwSwapBuffers(window.get());
        glfwPollEvents();

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
            std::cout << "OpenGL error: " << err << std::endl;
    }
}

void Window::cleanupImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Window::terminateWindow()
{
    glfwTerminate();
}

void Window::processInput(World* _world)
{
    if (inputManager)
        inputManager->processInput(deltaTime);
}
