#pragma once
//#include <GLFW/glfw3.h>
#include "../../world/world.hpp"
#include "../../render/sceneRenderer.hpp"

class InputManager
{
public:
    InputManager(GLFWwindow* window, SceneRenderer& renderer);

    void processInput(float deltaTime);
    void processMouseMovement(double xposIn, double yposIn);
    void processMouseScroll(double yoffset);

private:
    GLFWwindow* window = nullptr;
    World* world = nullptr;
    SceneRenderer& sceneRenderer;

    // Edge detection states
    bool escPrevPressed = false;
    bool spacePrevPressed = false;
    bool toggleGravityPrevPressed = false;
    bool spawnInteractablePrevPressed = false;
    bool interactPrevPressed = false;
    bool openPlayerInvPrevPressed = false;

    bool paused = false;

    // Mouse state
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;

    // Helpers
    bool isKeyPressedOnce(int key, bool& prevState);
    void moveCamera(const glm::vec3& direction, float deltaTime);
};
