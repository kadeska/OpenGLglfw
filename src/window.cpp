#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/Window.hpp"
//#include "../include/shaderProgram.hpp"
#include "../include/shader.hpp"
#include "../include/textureLoader.hpp"
#include "../include/camera3D.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

//#include "../include/vertexData.hpp"


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



    // glm test
   /* glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;*/

}

void Window::createShaderProgram()
{
    //shaderProgram = new ShaderProgram();
	//shaderProgram->makeShaderProgram();

    //Shader ourShader("", "");
	ourShader = new Shader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");
    ourShader->setUp();
	textureLoader.loadTextures(ourShader);

    
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

void Window::mainLoop()
{

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(window);

        // Rendering commands here
		// ----------------------
        render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::render() 
{
    // clear screen
	// ------

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate and Bind Textures
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLoader.texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureLoader.texture2);

	// Use shader program
	ourShader->use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    
    glm::mat4 projection = glm::perspective(glm::radians(myCamera->camZoomLevel), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader->setMat4("projection", projection);

    // camera/view transformation

    glm::mat4 view = myCamera->GetViewMatrix();
    ourShader->setMat4("view", view);

    // render cubes
	// ------------------------------

    glBindVertexArray(ourShader->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // no longer use glDrawElements as we no longer want to use an EBO
    // 
    
    for (unsigned int i = 0; i < ourShader->vertData.cubePositions.size(); i++) 
    {
        // calculate the model matrix for each object and pass it the shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ourShader->getCubeAt(i));
        float angle = 0; // 20.0f * i;
        //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

	//glBindVertexArray(0); // no need to unbind it every time
}

void Window::terminateWindow()
{
	// call deallocateResources from shaderProgram.cpp to free resources
	ourShader->deallocateResources();

    glfwTerminate();
}
//bool paused = false;
//bool keyPressed = false;
void Window::processInput(GLFWwindow* Window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    //    keyPressed = true; // is a key being pressed

    //    while (keyPressed) // while the key is pressed
    //    {
    //        if (!paused) // if the game is not paused
    //        {
    //            // pause the game
    //            paused = true;
				////reset the cursor
    //            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				//// set keyPressed to false to exit the loop
    //            keyPressed = false;
    //            return;
    //        }
    //        else
    //        {
    //            paused = false;
    //            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				//keyPressed = false;
    //            return;
    //        }
    //        exit;
    //    }
        
    }
        


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        myCamera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        myCamera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        myCamera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        myCamera->ProcessKeyboard(RIGHT, deltaTime);
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