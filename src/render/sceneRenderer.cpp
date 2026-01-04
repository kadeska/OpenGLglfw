#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include "../camera/camera3D.hpp"
#include "../shader/shader.hpp"
#include "../model/model.hpp"

#include "sceneRenderer.hpp"

#include "../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;
using LogLevel::DEBUG;

glm::mat4 projection;
glm::mat4 view;



// the vector of models to use for rendering each gameObject.
// Do not create multiple Model objects for the same model, 
// we only use these Model objects to get the meshes for rendering

// Positions of every Backpack model
std::vector<glm::vec3> backpackPositions = {
    { 5.0f, 3.0f, 1.0f },
    { 2.0f, 3.0f, -4.0f },
    { -3.0f, 3.0f, -2.0f },
    { 0.0f, 3.0f, 5.0f }
};

// Positions of every Donut model
std::vector<glm::vec3> donutPositions = {
    { 1.0f, 7.0f, 12.0f },
    { 10.0f, 2.0f, -1.0f }
};

SceneRenderer::SceneRenderer(unsigned int w, unsigned int h) : sceneWidth(w), sceneHeight(h)
{
    log("SceneRenderer constructor", DEBUG);
}

void SceneRenderer::loadModels()
{
    log("Loading models", DEBUG);

    models.push_back(std::make_unique<Model>("models/backpack/backpack.obj"));
    models.push_back(std::make_unique<Model>("models/donut/donut.obj"));
}

void SceneRenderer::initSceneRenderer()
{
    log("Inititalizing scene renderer", LogLevel::DEBUG);
    loadModels();
    initCamera();
    initSceneShader();
    populateRenderables();
	initialized = true;
}

void SceneRenderer::initCamera()
{
    log("Creating camera", DEBUG);

    camera = std::make_unique<Camera3D>(
        glm::vec3(0.0f, 2.0f, 10.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f
    );
}

void SceneRenderer::initSceneShader()
{
    log("Creating Scene Shader", DEBUG);

    sceneShader = std::make_unique<Shader>(
        "shaders/vertexShader.vs",
        "shaders/fragmentShader.fs"
    );
}

void SceneRenderer::initProjectionMatrix()
{
    projection = glm::perspective(
        glm::radians(camera->getCamZoom()),
        (float)sceneWidth / (float)sceneHeight,
        0.1f,
        100.0f
    );
}

void SceneRenderer::initViewMatrix()
{
    view = camera->GetViewMatrix();
    sceneShader->setMat4("projection", projection);
    sceneShader->setMat4("view", view);
}

void SceneRenderer::drawRenderables()
{
    for (const Renderable& r : renderables) {
        sceneShader->setMat4("model", r.transform);
        r.model->Draw(*sceneShader);
    }
}

void SceneRenderer::populateRenderables()
{
    log("Populating renderables", DEBUG);
    for (const glm::vec3& pos : backpackPositions)
    {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, pos);

        renderables.push_back({
            models[0].get(),
            transform
            });
    }
}

void SceneRenderer::useSceneShader()
{
    if (!sceneShader) 
    {
        log("Scene Shader is null", LogLevel::ERROR);
        return;
    }
    sceneShader->use();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void SceneRenderer::RenderScene()
{
    // clear color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    useSceneShader();
    initProjectionMatrix();
    initViewMatrix();
    drawRenderables();
}

Camera3D& SceneRenderer::getCamera()
{
    return *camera;
}

bool SceneRenderer::hasBeenInitialized() const
{
    return initialized;
}
