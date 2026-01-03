#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Window.hpp"
#include "input/inputManager.hpp"
#include "../render/sceneRenderer.hpp"
#include "../misc/programLogger.hpp"
#include "../misc/stateManager.hpp"


#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>



using ProgramLogger::log;
using ProgramLogger::LogLevel;
using namespace StateManager;


//GLFWwindow* win = nullptr;
World* world = nullptr;

// --------------------------------------------------------
// GLFW Callbacks
// --------------------------------------------------------
namespace {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        if (ImGui::GetIO().WantCaptureKeyboard)
            return;

        if (ImGui::GetIO().WantCaptureKeyboard)
            return;

        // Your game input handling
    }


    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (!window) return;

        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        if (ImGui::GetIO().WantCaptureMouse)
            return;

        auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (inputManager && !ImGui::GetIO().WantCaptureMouse)
            inputManager->processMouseMovement(xpos, ypos);
    }


    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

        if (ImGui::GetIO().WantCaptureMouse)
            return;

        auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (inputManager)
            inputManager->processMouseScroll(yoffset);
    }


    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        if (ImGui::GetIO().WantCaptureMouse)
            return;

        if (ImGui::GetIO().WantCaptureMouse)
            return;

        // Your input logic here
    }


    //void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    //{
    //    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    //    {
    //        double xpos, ypos;
    //        //getting cursor position
    //        glfwGetCursorPos(window, &xpos, &ypos);
    //        std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
    //    }
    //}

} // namespace

Window::Window(StateManager::GameStateManager& _gameStateManager) : gameState(_gameStateManager)
{
    log("Window constructor", LogLevel::DEBUG);
}

// --------------------------------------------------------
// Window Methods
// --------------------------------------------------------
void Window::initialize(float _camX, float _camY, float _camZ)
{
    log("Window initializer", LogLevel::DEBUG);
    if (!loadingRenderer) 
    {
        log("Woops, you forgot to set loadingRenderer before initializing the window. It is null.", LogLevel::ERROR);
    }

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
    log("Creating window", LogLevel::DEBUG);
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

    //// 2. Initialize sceneRenderer first
    //sceneRenderer = std::make_unique<SceneRenderer>(SCR_WIDTH, SCR_HEIGHT);

    //// 3. Initialize InputManager
    //inputManager = std::make_unique<InputManager>(window.get(), *sceneRenderer);

    //// 4. Attach InputManager pointer to window user pointer for callbacks
    //glfwSetWindowUserPointer(window.get(), inputManager.get());

    //// 5. Set GLFW callbacks (now safe, because objects exist)
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
    glfwSetCursorPosCallback(window.get(), mouse_callback);
    glfwSetScrollCallback(window.get(), scroll_callback);
    glfwSetMouseButtonCallback(window.get(), mouse_button_callback);
    glfwSetKeyCallback(window.get(), key_callback);


    //// 6. Set cursor input mode
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    initImGui();

    //log("Window created and callbacks set", LogLevel::DEBUG);
    log("Window created", LogLevel::DEBUG);
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

void Window::mainLoop()
{
    if (!loadingRenderer) 
    {
        log("loadingRenderer is null", LogLevel::ERROR);
    }

    

    while (!glfwWindowShouldClose(getGLFWwindow())) {
        clearColor();
        //log("Game state is: " + std::to_string(static_cast<int>(gameState.getState())), LogLevel::DEBUG);

        switch (gameState.getState()) {

        case GameState::LOADING:
            //log("GameState is LOADING", LogLevel::DEBUG);
            
            loadingRenderer->render();
            
            break;

        case GameState::PLAYING:
            //log("GameState is PLAYING", LogLevel::DEBUG);
            //world->update();
            //world->render();
            break;
        }


        pollEvents();        // Input first
        startImGuiFrame();  // Then build ImGui
        renderImGui();      // Then render ImGui
        swapBuffers();


        // Fake loading completion for now
        //if (gameState.is(GameState::LOADING)) {
        //    gameState.setState(GameState::PLAYING);
        //}


        GLenum err;
                while ((err = glGetError()) != GL_NO_ERROR)
                    std::cout << "OpenGL error: " << err << std::endl;
    }
}

void Window::setLoadingRenderer(LoadingScreenRenderer* _renderer)
{
    loadingRenderer = _renderer;
}

void Window::terminateWindow()
{
    loadingRenderer->cleanup();
    glfwTerminate();
}

void Window::clearColor()
{
	if (gameState.is(GameState::LOADING))
    {
        glClearColor(1.2f, 1.3f, 1.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return;
    }
    
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::processInput(World* _world)
{
    if (!inputManager)
    {
        log("InputManager is null...", LogLevel::ERROR);
    }

	inputManager->checkESCToggle();
    inputManager->processInput(deltaTime);
}

void Window::initImGui()
{
    // Create ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls

    // Setup style
    ImGui::StyleColorsDark(); // or ImGui::StyleColorsClassic();

    // Setup platform/renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplGlfw_InitForOpenGL(window.get(), false);
    ImGui_ImplOpenGL3_Init("#version 330"); // GLSL version
}

void Window::renderImGui()
{
    // Example: create a simple window
    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("This is some debug text.");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // Render ImGui to OpenGL
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::startImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::terminateImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
