#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include "../include/model/model.hpp"
#include "../include/Window.hpp"
#include "../include/shader.hpp"
#include "../include/textureLoader.hpp"
#include "../include/textRenderer.hpp"
#include "../include/camera3D.hpp"
#include "../include/vertexData.hpp"
VertexData vertData;

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;





GLFWwindow* window;
Shader* sceneShader;
Shader* textShader;

Model* backpackModel = nullptr;


TextRenderer textRenderer;

TextureLoader textureLoader;


std::string fontFile = "fonts/arial.ttf";
std::string pausedText = "Paused";
std::string interactText = "Interact using E";
glm::vec3 textLoc = glm::vec3(5.0f, 2.0f, 3.0f);

// inventory rendering stuff
std::string inventoryData;
std::string playerInvData = "Testing player inventory";

// camera stuff

Camera3D* myCamera;
const float YAW = 45.0f;
const float PITCH = -30.0f;
float lastX;
float lastY;
bool firstMouse = true;

// timing

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


void Window::initialize(float _camX, float _camY, float _camZ)
{

    myCamera = new Camera3D(glm::vec3(_camX, _camY, _camZ), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);

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

void Window::imGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::createShaderProgram()
{
    sceneShader = new Shader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");
	textShader = new Shader("shaders/textVertexShader.vs", "shaders/textFragmentShader.fs");
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
    // IMGUI stuff
    // -----------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // frame time stuff
    // ------------------

    //double previousTime;
    //glfwSetTime(0.0);
    //previousTime = glfwGetTime();
    //int frameCount = 0;


	backpackModel = new Model("models/backpack/backpack.obj");
	//backpackModel = new Model("models/donut/donut.obj");
	//backpackModel = new Model("models/modeltest/donut_icing6.obj");


    // render loop
    // -----------
    //sceneShader->setUp();
    glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // Measure speed
        //double currentTime = glfwGetTime();
        //frameCount++;
        // If a second has passed.
        //if (currentTime - previousTime >= 1.0)
        //{
        //    // Display the frame count here any way you want.
        //    log("FPS: " + std::to_string(frameCount));

        //    frameCount = 0;
        //    previousTime = currentTime;
        //}

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

		// set player position after updating camera position, 
        // Players location must be set before world update
        // ----------------------------------

		_world->setPlayerPos(myCamera->getCamPos());

        //_world->spawnPlayer(myCamera->getCamPos());

        // update world
        // ----------------------------------------

        _world->updateWorld();

        // Rendering commands here
		// ----------------------------------------------

        //glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT);

        //imGuiNewFrame();

        // draw 3d scene
        // ----------------------------------------------------

        //backpackModel->Draw(*sceneShader);
        renderScene(_world, sceneShader);
		

		// draw text over the scene
        // --------------------------------------------------------

        //renderTextOverlays(_world);

        // draw imGui overlay
        // --------------------------------------------------------
        //renderImGuiOverlay(_world);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }
    }
	cleanupImGui();
    //glfwTerminate();
}

void Window::renderScene(World*& _world, Shader* _shader)
{
    // per-frame time logic
        // --------------------
    //float currentFrame = static_cast<float>(glfwGetTime());
    //deltaTime = currentFrame - lastFrame;
    //lastFrame = currentFrame;

    // input
    // -----
    //processInput(window, _world);

    // render
    // ------
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    _shader->use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(myCamera->getCamZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = myCamera->GetViewMatrix();
    _shader->setMat4("projection", projection);
    _shader->setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    _shader->setMat4("model", model);


    backpackModel->Draw(_shader);


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    //glfwSwapBuffers(window);
    //glfwPollEvents();


 //   glFrontFace(GL_CW);

	//// Use shader program
	//sceneShader->use();

 //   // pass projection matrix to shader (note that in this case it could change every frame)
 //   
 //   glm::mat4 projection = glm::perspective(glm::radians(myCamera->getCamZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
 //   sceneShader->setMat4("projection", projection);

 //   // camera/view transformation

 //   glm::mat4 view = myCamera->GetViewMatrix();
 //   sceneShader->setMat4("view", view);

 //   glBindVertexArray(sceneShader->VAO);

 //   // render objects
	//// ------------------------------
 //   
 //   for (EntityCube* cube : _world->getArrayOfCubes())
 //   {
 //       glActiveTexture(GL_TEXTURE0);
 //       glBindTexture(GL_TEXTURE_2D, cube->getTexID()); 
 //       glm::mat4 model = glm::mat4(1.0f);
 //       model = glm::translate(model, cube->getEntityPosition()); 
 //       sceneShader->setMat4("model", model);
 //       glBindVertexArray(sceneShader->VAO);
 //       glDrawArrays(GL_TRIANGLES, 0, 36);

 //   }
 //   for (EntityChest* chest : _world->getArrayOfChests())
 //   {
 //       glActiveTexture(GL_TEXTURE0);
 //       glBindTexture(GL_TEXTURE_2D, chest->getTextureID());
 //       chest->getTexID();
 //       glm::mat4 model = glm::mat4(1.0f);
 //       model = glm::translate(model, chest->getEntityPosition());
 //       sceneShader->setMat4("model", model);
 //       glBindVertexArray(sceneShader->VAO);
 //       glDrawArrays(GL_TRIANGLES, 0, 36);

 //   }
    //
}

void Window::renderTextOverlays(World*& _world)
{
    if (paused)
    {
        textRenderer.renderText(pausedText, textShader, textLoc, (SCR_WIDTH / 2.0f) - 100, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, fontFile);
        return;
    }

    if (_world->getInRangeOfInteracable())
    {
        textRenderer.renderText(interactText, textShader, textLoc, (SCR_WIDTH / 2.0f) - 100, SCR_HEIGHT / 2.0f, 1.0f, SCR_WIDTH, SCR_HEIGHT, fontFile);
    }
}

void Window::renderImGuiOverlay(World*& _world)
{
    // player inventory
    if (_world->getShoudRenderPlayerInventory()) 
    {
        ImVec2 text_size = ImGui::CalcTextSize(playerInvData.c_str(), nullptr, false, 0.0f);
        ImVec2 window_position((SCR_WIDTH * 0.5) - 200, (SCR_HEIGHT * 0.5) + 190);
        ImGui::SetNextWindowPos(window_position);
        int line_count = text_size.y / ImGui::GetTextLineHeight();
        ImGui::SetNextWindowSize(ImVec2(text_size.x + 200.0f, (line_count + 1) * ImGui::GetTextLineHeight() + 50.0f));

        ImGui::Begin("Player Inventory");
        //ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", myCamera->getCamPos().x, myCamera->getCamPos().y, myCamera->getCamPos().z);
        ImGui::Text(playerInvData.c_str());
        ImGui::End();
    }

    // other inventories
    if (_world->getShouldRenderInventory())
    {
        ImVec2 text_size = ImGui::CalcTextSize(inventoryData.c_str(), nullptr, false, 0.0f);
        ImVec2 window_position((SCR_WIDTH * 0.5) - 380, (SCR_HEIGHT * 0.5));
        ImGui::SetNextWindowPos(window_position);
        int line_count = text_size.y / ImGui::GetTextLineHeight();
        ImGui::SetNextWindowSize(ImVec2(text_size.x + 30.0f, (line_count + 1) * ImGui::GetTextLineHeight() + 30.0f));

        ImGui::Begin("Inventory");
        //ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", myCamera->getCamPos().x, myCamera->getCamPos().y, myCamera->getCamPos().z);
        ImGui::Text(inventoryData.c_str());
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::cleanupImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Window::terminateWindow()
{
	// call deallocateResources from shaderProgram.cpp to free resources
	textShader->deallocateResources();

    glfwTerminate();
}

Shader* Window::getSceneShader()
{
    return sceneShader;
}

Shader* Window::getTextShader()
{
    return textShader;
}

void Window::processInput(GLFWwindow*& _window, World*& _world)
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

	// if paused, skip rest of input processing
    if (paused) { return; }

    // SPACE key edge detection for spawning cube

    bool spacePressed = glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spacePressed && !spacePrevPressed) // Only on transition from not pressed to pressed
    {
        //log("Space key pressed");
        _world->createCube(_world->getPlayerPos() + glm::vec3(2.0f, 0.0f, 0.0f));
    }
    spacePrevPressed = spacePressed; // Update previous state


	// G key edge detection for toggling gravity

    bool toggleGravity = glfwGetKey(_window, GLFW_KEY_G) == GLFW_PRESS;
    if (toggleGravity && !toggleGravityPressed) 
    {
        //log("Gravity toggled");
        //myCamera->useGravity = true;
        if (!myCamera->getUseGravity()) 
        {
            myCamera->setUseGravity(true);
            log("Gravity toggled on");
        } 
        else 
        {
            myCamera->setUseGravity(false);
            log("Gravity toggled off");
        }
    }
    toggleGravityPressed = toggleGravity;


	// C key edge detection for spawning a chest

    bool spawnInteractable = glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS;
    if (spawnInteractable && !spawnInteractablePressed)
    {
        
        _world->createChest(_world->getPlayerPos() + glm::vec3(2.0f, 0.0f, 0.0f));
    }
    spawnInteractablePressed = spawnInteractable;
    

    // E key edge detection for interacting with objects

    bool interact = glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS;
    if (interact && !interactPressed)
    {
        //log("Interacting with object");
        //_world->interactWithObjectInRange(dataToRenderAsText);
        if (_world->getClosestChest() == nullptr) 
        {
            // if the chest is null then just return, dont do anything. Player is not in range of any chest
            return;
        }

        _world->getClosestChest()->openInventory(inventoryData);

        /*if (_world->getClosestChest()->openInventory(inventoryData)) 
        {

        }*/
    }
    interactPressed = interact;


    // TAB key edge detection for opening player inventory

    bool openPlayerInv = glfwGetKey(_window, GLFW_KEY_TAB) == GLFW_PRESS;
    if (openPlayerInv && !openPlayerInvPressed)
    {
        log("TAB pressed");
        _world->togglePlayerInventory();
    }
    openPlayerInvPressed = openPlayerInv;




    // Movement keys (continuous)
    // ------------------------------

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        //log("W key pressed");
        float velocity = myCamera->getCamMovementSpeed() * deltaTime;
        glm::vec3 moveVec = myCamera->getCamFront() * velocity;
        glm::vec3 intendedPos = myCamera->getCamPos() + moveVec;
        glm::vec3 checkPos = intendedPos + glm::normalize(myCamera->getCamFront()) * myCamera->getCollisionRadius();
        glm::vec3 gridCheckPos = _world->getEntityManager()->snapToGrid(checkPos);  // <-----  snapToGrid and isPositionOccupied can be put in a helper class
        if (!_world->getEntityManager()->isPositionOccupied(gridCheckPos)) {
            myCamera->setCamPos(intendedPos);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        float velocity = myCamera->getCamMovementSpeed() * deltaTime;
        glm::vec3 moveVec = -myCamera->getCamFront() * velocity;
        glm::vec3 intendedPos = myCamera->getCamPos() + moveVec;
        glm::vec3 checkPos = intendedPos - glm::normalize(myCamera->getCamFront()) * myCamera->getCollisionRadius();
        glm::vec3 gridCheckPos = _world->getEntityManager()->snapToGrid(checkPos);
        if (!_world->getEntityManager()->isPositionOccupied(gridCheckPos)) {
            myCamera->setCamPos(intendedPos);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        float velocity = myCamera->getCamMovementSpeed() * deltaTime;
        glm::vec3 moveVec = -myCamera->getCamRight() * velocity;
        glm::vec3 intendedPos = myCamera->getCamPos() + moveVec;
        glm::vec3 checkPos = intendedPos - glm::normalize(myCamera->getCamRight()) * myCamera->getCollisionRadius();
        glm::vec3 gridCheckPos = _world->getEntityManager()->snapToGrid(checkPos);
        if (!_world->getEntityManager()->isPositionOccupied(gridCheckPos)) {
            myCamera->setCamPos(intendedPos);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        float velocity = myCamera->getCamMovementSpeed() * deltaTime;
        glm::vec3 moveVec = myCamera->getCamRight() * velocity;
        glm::vec3 intendedPos = myCamera->getCamPos() + moveVec;
        glm::vec3 checkPos = intendedPos + glm::normalize(myCamera->getCamRight()) * myCamera->getCollisionRadius();
        glm::vec3 gridCheckPos = _world->getEntityManager()->snapToGrid(checkPos);
        if (!_world->getEntityManager()->isPositionOccupied(gridCheckPos)) {
            myCamera->setCamPos(intendedPos);
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