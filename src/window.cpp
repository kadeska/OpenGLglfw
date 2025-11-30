#include <iostream>
//#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "../include/Window.hpp"
#include "../include/shader.hpp"
#include "../include/textureLoader.hpp"
#include "../include/camera3D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../include/programLogger.hpp"
using ProgramLogger::log;

#include "../include/vertexData.hpp"
VertexData vertData;

// camera stuff

Camera3D* myCamera;// = new Camera3D(glm::vec3(CAM_X, CAM_Y, CAM_Z));
float lastX;
float lastY;
bool firstMouse = true;

// timing

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void Window::initialize(float _camX, float _camY, float _camZ)
{

    myCamera = new Camera3D(glm::vec3(_camX, _camY, _camZ));

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
}

void Window::createShaderProgram()
{
    sceneShader = new Shader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");
	textShader = new Shader("shaders/textVertexShader.vs", "shaders/textFragmentShader.fs");
    //ourShader->setUp();
	//textureLoader.loadTextures(sceneShader);
}

void Window::loadTextures()
{
    textureLoader.loadTextures(sceneShader);
}

void Window::createWindow()
{
    // glfw Window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return; // -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::loadOpenGL()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;// -1;
    }
}

void Window::mainLoop(World* _world)
{
    sceneShader->setUp();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // ----------------------
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -------------------------

        processInput(window, _world);

        // camer stuff
        // -----------------------------

		myCamera->applyGravity(deltaTime);
		myCamera->updatePosition(deltaTime);

		// set player position after updating camera position
        // ----------------------------------

		_world->setPlayerPos(myCamera->camPos);

        // update world
        // ----------------------------------------

        _world->updateWorld();

        // Rendering commands here
		// ----------------------------------------------

        glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT);

        // draw 3d scene
        // ----------------------------------------------------

        renderScene(_world);

		// draw text over the scene
        // --------------------------------------------------------

        renderTextOverlays(_world);
        

        //textRenderer.renderText("This is sample text 1", textShader, glm::vec3(2.0f, 0.0f, 0.0f), 0.0f, 100.5f, 1, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");
        //textRenderer.renderText("This is sample text 2", textShader, glm::vec3(20.0f, 0.0f, 0.0f), 0.0f, 50.5f, 1, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }
    }
}

void Window::renderScene(World*& _world)
{
    glFrontFace(GL_CW);

	// Use shader program
	sceneShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    
    glm::mat4 projection = glm::perspective(glm::radians(myCamera->camZoomLevel), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    sceneShader->setMat4("projection", projection);

    // camera/view transformation

    glm::mat4 view = myCamera->GetViewMatrix();
    sceneShader->setMat4("view", view);

    glBindVertexArray(sceneShader->VAO);

    // render objects
	// ------------------------------
    
    for (EntityCube& cube : _world->getEntityCubes())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube.getTexID()); 
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cube.getEntityPosition()); 
        sceneShader->setMat4("model", model);
        glBindVertexArray(sceneShader->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    }
    for (EntityChest& chest : _world->getEntityChests())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, chest.getTexID());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, chest.getEntityPosition());
        sceneShader->setMat4("model", model);
        glBindVertexArray(sceneShader->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    }
}

void Window::renderTextOverlays(World*& _world)
{
    if (paused)
    {
        textRenderer.renderText("Paused", textShader, glm::vec3(5.0f, 2.0f, 3.0f), (SCR_WIDTH / 2.0f) - 100, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");
    }

    if (_world->getInRangeOfInteracable())
    {
        textRenderer.renderText("Interact using E", textShader, glm::vec3(5.0f, 2.0f, 3.0f), (SCR_WIDTH / 2.0f) - 100, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");
    }

    int index = 10;

    //textRenderer.renderText("Testing1", textShader, glm::vec3(5.0f, 2.0f, 3.0f), (SCR_WIDTH / 2.0f) - 100, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");

    // draw inventory
    for (EntityChest& chest: _world->getEntityChests()) 
    {
        //textRenderer.renderText("Testing2", textShader, glm::vec3(5.0f, 2.0f, 3.0f), (SCR_WIDTH / 2.0f) - 100, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");
        if (chest.getChestInventory().showInv()) 
        {
            //textRenderer.renderText("Testing3", textShader, glm::vec3(5.0f, 2.0f, 3.0f), (SCR_WIDTH / 2.0f) - 150, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");
            for (ItemType item: chest.getChestInventoryItems()) 
            {
                textRenderer.renderText("Testing4", textShader, glm::vec3(5.0f, 2.0f, 3.0f), (SCR_WIDTH / 2.0f) - 200, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, "fonts/arial.ttf");

                index += 10;
            }
        }
    }
}


void Window::terminateWindow()
{
	// call deallocateResources from shaderProgram.cpp to free resources
	textShader->deallocateResources();

    glfwTerminate();
}

void Window::processInput(GLFWwindow* _window, World* _world)
{
    // ESC key edge detection

    bool escPressed = glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (escPressed && !escPrevPressed) // Only on transition from not pressed to pressed
    {
        if (!paused)
        {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            paused = true;
        }
        else
        {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			firstMouse = true; // to prevent sudden camera jump.  // Needs fixing, doesnt work like expected. Maybe store lastX and lastY when pausing?
            paused = false;
        }
    }
    escPrevPressed = escPressed; // Update previous state

    // SPACE key edge detection for spawning cube

    bool spacePressed = glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spacePressed && !spacePrevPressed) // Only on transition from not pressed to pressed
    {
        //log("Space key pressed");
        _world->spawnEntityCubeAt(_world->getPlayerPos() + glm::vec3(2.0f, 0.0f, 0.0f));
    }
    spacePrevPressed = spacePressed; // Update previous state


	// G key edge detection for toggling gravity

    bool toggleGravity = glfwGetKey(_window, GLFW_KEY_G) == GLFW_PRESS;
    if (toggleGravity && !toggleGravityPressed) 
    {
        //log("Gravity toggled");
        //myCamera->useGravity = true;
        if (!myCamera->useGravity) 
        {
            myCamera->useGravity = true;
            log("Gravity toggled on");
        } 
        else 
        {
            myCamera->useGravity = false;
            log("Gravity toggled off");
        }
    }
    toggleGravityPressed = toggleGravity;


	// C key edge detection for spawning a chest

    bool spawnInteractable = glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS;
    if (spawnInteractable && !spawnInteractablePressed)
    {
        
        _world->spawnChestAt(_world->getPlayerPos() + glm::vec3(2.0f, 0.0f, 0.0f));
    }
    spawnInteractablePressed = spawnInteractable;
    

    // E key edge detection for interacting with objects

    bool interact = glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS;
    if (interact && !interactPressed)
    {

        //log("Interacting with object");
        _world->interactWithObjectInRange();
    }
    interactPressed = interact;


    // Movement keys (continuous)
    // ------------------------------

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        float velocity = myCamera->camMovementSpeed * deltaTime;
        glm::vec3 moveVec = myCamera->camFront * velocity;
        glm::vec3 intendedPos = myCamera->camPos + moveVec;
        glm::vec3 checkPos = intendedPos + glm::normalize(myCamera->camFront) * myCamera->collisionRadius;
        glm::vec3 gridCheckPos = _world->snapToGrid(checkPos);
        if (!_world->isPositionOccupied(gridCheckPos)) {
            myCamera->camPos = intendedPos;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        float velocity = myCamera->camMovementSpeed * deltaTime;
        glm::vec3 moveVec = -myCamera->camFront * velocity;
        glm::vec3 intendedPos = myCamera->camPos + moveVec;
        glm::vec3 checkPos = intendedPos - glm::normalize(myCamera->camFront) * myCamera->collisionRadius;
        glm::vec3 gridCheckPos = _world->snapToGrid(checkPos);
        if (!_world->isPositionOccupied(gridCheckPos)) {
            myCamera->camPos = intendedPos;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        float velocity = myCamera->camMovementSpeed * deltaTime;
        glm::vec3 moveVec = -myCamera->camRight * velocity;
        glm::vec3 intendedPos = myCamera->camPos + moveVec;
        glm::vec3 checkPos = intendedPos - glm::normalize(myCamera->camRight) * myCamera->collisionRadius;
        glm::vec3 gridCheckPos = _world->snapToGrid(checkPos);
        if (!_world->isPositionOccupied(gridCheckPos)) {
            myCamera->camPos = intendedPos;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        float velocity = myCamera->camMovementSpeed * deltaTime;
        glm::vec3 moveVec = myCamera->camRight * velocity;
        glm::vec3 intendedPos = myCamera->camPos + moveVec;
        glm::vec3 checkPos = intendedPos + glm::normalize(myCamera->camRight) * myCamera->collisionRadius;
        glm::vec3 gridCheckPos = _world->snapToGrid(checkPos);
        if (!_world->isPositionOccupied(gridCheckPos)) {
            myCamera->camPos = intendedPos;
        }
    }
        
}

void framebuffer_size_callback(GLFWwindow* Window, int width, int height)
{
    // make sure the viewport matches the new Window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    myCamera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    myCamera->ProcessMouseScroll(static_cast<float>(yoffset));
}