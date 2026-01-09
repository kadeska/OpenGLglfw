#include "loadingScreenRenderer.hpp"
#include "../window/window.hpp"
#include "../shader/shader.hpp"
#include "textRenderer.hpp"
#include "../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

static Shader* shader = nullptr;
static TextRenderer* textRenderer = nullptr;

std::string text = "Loading...";
std::string path = "fonts/arial.ttf";
glm::vec3 color = {12, 0, 0};

LoadingScreenRenderer::LoadingScreenRenderer(Window* _window) : window(_window)
{
    log("LoadingScreenRenderer constructor", LogLevel::DEBUG);
}

void LoadingScreenRenderer::init() 
{
    log("Initializing shader and text renderer...", LogLevel::DEBUG);
    shader = new Shader("shaders/textVertexShader.vs", "shaders/textFragmentShader.fs");
    textRenderer = new TextRenderer();
    if (!textRenderer->init(path, shader, window->getWindowSize().x, window->getWindowSize().y)) 
    {
        log("Failed to initialize textRenderer", LogLevel::ERROR);
    }
}

void LoadingScreenRenderer::render()
{
    //log("render text", LogLevel::DEBUG);
    textRenderer->renderText(text, color, window->getWindowSize().x / 2 - 80, window->getWindowSize().y / 2, 1.0f);
}

void LoadingScreenRenderer::cleanup()
{
    textRenderer->~TextRenderer();
}
