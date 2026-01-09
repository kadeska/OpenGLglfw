#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include "../camera/camera3D.hpp"
#include "../shader/shader.hpp"
#include "../model/model.hpp"

#include "sceneRenderer.hpp"

#include "../physics/collisionManager.hpp"

#include "../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;
using LogLevel::DEBUG;

//glm::mat4 transform;
glm::mat4 projection;
glm::mat4 view;

// Gravity
double GRAVITY = -0.067;

GameObject* physicsBackpack = nullptr;
CollisionManager* collisionManager = nullptr;


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

std::vector<glm::vec3> groundPlanePositions =
{
    {0,0,0}
};

SceneRenderer::SceneRenderer(unsigned int w, unsigned int h) : sceneWidth(w), sceneHeight(h)
{
    log("SceneRenderer constructor", DEBUG);
    collisionManager = new CollisionManager();
}

void SceneRenderer::loadModels()
{
    log("Loading models", DEBUG);

    models.push_back(new Model("models/backpack/backpack.obj", 0));
    models.push_back(new Model("models/donut/donut.obj", 1));
    models.push_back(new Model("models/plane/BasicFlatPlane_20x20.obj", 2));

}

void SceneRenderer::addGameObject(GameObject* _gameObject)
{
    log("Adding game object", LogLevel::DEBUG);
    gameObjects.push_back(_gameObject);
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
    for (const Renderable::Renderable* r : renderables) {
        sceneShader->setMat4("model", r->transform);
        r->model->Draw(*sceneShader);
    }
}

// Add every renderable gameObject from the world to the list of things to render 
void SceneRenderer::populateRenderables()
{
    log("Populating renderables", DEBUG);

    for (Model* m : models) 
    {
        unsigned int id = m->getID();
        switch (id)
        {
        case 0:
            log("Backpack", LogLevel::RENDERABLE);
            addRenderables(backpackPositions, id);
            break;
        case 1:
            log("Donut", LogLevel::RENDERABLE);
            addRenderables(donutPositions, id);
            break;
        case 2:
            log("GroundPlane", LogLevel::RENDERABLE);
            addRenderables(groundPlanePositions, id);
            break;

        default:
            break;
        }
    }

    // populate gameObjects
    log("Populating gameObjects", DEBUG);

    for (Renderable::Renderable* r : renderables) 
    {
        glm::vec3& pos = r->position;
        unsigned int id = r->model->getID();
        switch (id)
        {
        case 0:
            addGameObject(new GameObject(pos, r));
            break;
        case 1:
            addGameObject(new GameObject(pos, r));
            break;
        case 2:
            addGameObject(new GameObject(pos, r));
            break;
        default:
            break;
        }
    }

    gameObjects[0]->setUseGravity(true); // for testing the flag
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

void SceneRenderer::updateRenderables(float _deltaTime)
{
    // do update logic first, such as collision check, then applying gravity or velocity if allowed, 

    for (GameObject* go : gameObjects) 
    {
        // if the gameObject has a collision object such as sphere then calculate collisions
        if (go->getCollisionSphere() != nullptr) 
        {
            // check for collision
            if (!collisionManager->checkForCollisions(go, nullptr))
            {
                // collision not detected, move
                if (go->getUseGravity()) 
                {
                    // apply gravity
                    go->setVelocity(go->getVelocity() + glm::vec3(0, GRAVITY, 0) * _deltaTime);
                    go->setPosition(go->getPosition() + go->getVelocity() * _deltaTime);
                }
            }
        }
        // now translate the randerable based on update logic
        // rebuild transform from scratch
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, go->getPosition());
        go->getRenderable()->transform = transform;
    }
}

void SceneRenderer::addRenderables(std::vector<glm::vec3> _pos, unsigned int _id)
{
    
    for (glm::vec3 pos : _pos) 
    {
        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, pos);
        // This will break if I dont have all the expected models loaded
        renderables.push_back(new Renderable::Renderable(models[_id], transform, pos)); //  Need to fix, id should not directly index into the models array like this. Need to make a type.
    }
}

//int y = 0;
void SceneRenderer::RenderScene(float _deltaTime)
{
    // clear color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    updateRenderables(_deltaTime);
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

