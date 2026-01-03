#include <GLFW/glfw3.h>

#include "inputManager.hpp"

#include <imgui/imgui.h>

#include "../../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

InputManager::InputManager(GLFWwindow* _window, SceneRenderer& _renderer)
    : window(_window), sceneRenderer(_renderer)
{
}

// -----------------------------
// Key edge detection helper
// -----------------------------
bool InputManager::isKeyPressedOnce(int key, bool& prevState)
{
    bool pressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool triggered = pressed && !prevState;
    prevState = pressed;
    return triggered;
}

// -----------------------------
// Movement with collision
// -----------------------------
void InputManager::moveCamera(const glm::vec3& direction, float deltaTime)
{
    float velocity = sceneRenderer.getCamera().getCamMovementSpeed() * deltaTime;
    glm::vec3 intendedPos = sceneRenderer.getCamera().getCamPos() + direction * velocity;
    glm::vec3 checkPos = intendedPos + glm::normalize(direction) * sceneRenderer.getCamera().getCollisionRadius();
    glm::vec3 gridCheckPos = world->getEntityManager()->snapToGrid(checkPos);

    if (!world->getEntityManager()->isPositionOccupied(gridCheckPos))
    {
        sceneRenderer.getCamera().setCamPos(intendedPos);
    }
}

// -----------------------------
// Keyboard input
// -----------------------------
void InputManager::processInput(float deltaTime)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

	//checkESCToggle();

    if (paused) return; // skip rest of input if paused

    // check for valid world pointer.
	// do this after pause check to allow pausing even if world is null,
    // this lets me get the cursor back for closing the window.

    if (world == nullptr)
    {
        log("World pointer is null in InputManager::processInput", LogLevel::ERROR);
        return;
    }



    // ----------------------------
    // SPACE: Spawn cube
    // ----------------------------
    if (isKeyPressedOnce(GLFW_KEY_SPACE, spacePrevPressed))
        world->createCube(world->getPlayerPos() + glm::vec3(2.0f, 0.0f, 0.0f));

    // ----------------------------
    // G: Toggle gravity
    // ----------------------------
    if (isKeyPressedOnce(GLFW_KEY_G, toggleGravityPrevPressed))
    {
        bool useGravity = sceneRenderer.getCamera().getUseGravity();
        sceneRenderer.getCamera().setUseGravity(!useGravity);
        log(std::string("Gravity toggled ") + (!useGravity ? "on" : "off"), LogLevel::DEBUG);
    }

    // ----------------------------
    // C: Spawn chest
    // ----------------------------
    if (isKeyPressedOnce(GLFW_KEY_C, spawnInteractablePrevPressed))
        world->createChest(world->getPlayerPos() + glm::vec3(2.0f, 0.0f, 0.0f));

    // ----------------------------
    // E: Interact with closest chest
    // ----------------------------
    if (isKeyPressedOnce(GLFW_KEY_E, interactPrevPressed))
    {
        log("Spawn chest");
        /*if (Chest* closest = world.getClosestChest())
            closest->openInventory();*/
    }

    // ----------------------------
    // TAB: Toggle player inventory
    // ----------------------------
    if (isKeyPressedOnce(GLFW_KEY_TAB, openPlayerInvPrevPressed))
        world->togglePlayerInventory();

    // ----------------------------
    // Movement (continuous)
    // ----------------------------
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveCamera(sceneRenderer.getCamera().getCamFront(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveCamera(-sceneRenderer.getCamera().getCamFront(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveCamera(-sceneRenderer.getCamera().getCamRight(), deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveCamera(sceneRenderer.getCamera().getCamRight(), deltaTime);
}

void InputManager::checkESCToggle()
{
    // ----------------------------
    // ESC: Pause toggle
    // ----------------------------
    if (isKeyPressedOnce(GLFW_KEY_ESCAPE, escPrevPressed))
    {
        paused = !paused;
        glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        firstMouse = true; // Reset mouse on pause toggle
    }
}

// -----------------------------
// Mouse movement
// -----------------------------
void InputManager::processMouseMovement(double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    sceneRenderer.getCamera().ProcessMouseMovement(xoffset, yoffset);
}

// -----------------------------
// Mouse scroll
// -----------------------------
void InputManager::processMouseScroll(double yoffset)
{
    sceneRenderer.getCamera().ProcessMouseScroll(static_cast<float>(yoffset));
}
