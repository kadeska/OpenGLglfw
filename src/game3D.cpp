#include "../include/game3D.hpp"

//#include "../include/shader.hpp"




Window myWindow;
World* myWorld;

static const int WORLD_SIZE = 24;
static const float WORLD_SEED = 0;

static const float CAM_X = 0.0f;
static const float CAM_Y = 10.0f;
static const float CAM_Z = 0.0f;


Game3D::Game3D()
{
	// since window has no constructor, we don't need to instantiate it
    //myWindow = Window();
	myWindow.initialize(CAM_X, CAM_Y, CAM_Z);
	myWindow.createWindow();
	myWindow.loadOpenGL();
	myWindow.createShaderProgram();
	myWindow.loadTextures();

	// Create world
	myWorld = new World(myWindow.sceneShader, WORLD_SEED, WORLD_SIZE);
	//myWorld->createWorld(0);
	myWorld->generateWorld(WORLD_SEED);
}

Game3D::~Game3D()
{
}

void Game3D::start()
{
	myWindow.mainLoop(myWorld);
}

void Game3D::stop()
{
	myWindow.terminateWindow();
}
