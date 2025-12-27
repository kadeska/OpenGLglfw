#include "../include/game3D.hpp"
#include "../include/window.hpp"
#include "../include/world.hpp"


Window myWindow;
World* myWorld;

static const int WORLD_SIZE = 24;
static const float WORLD_SEED = 0;

static const float PLAYER_X = 5.0f;
static const float PLAYER_Y = 2.0f;
static const float PLAYER_Z = 5.0f;


Game3D::Game3D()
{
	// since window has no constructor, we don't need to instantiate it
    //myWindow = Window();
	myWindow.initialize(PLAYER_X, PLAYER_Y, PLAYER_Z);
	myWindow.createWindow();
	myWindow.loadOpenGL();
	myWindow.createShaderProgram();
	myWindow.loadTextures();

	// Create world
	myWorld = new World(myWindow.getSceneShader(), WORLD_SEED, WORLD_SIZE);
	myWorld->createWorld(WORLD_SEED);
	myWorld->spawnPlayer({PLAYER_X, PLAYER_Y, PLAYER_Z});
	//myWorld->test();
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
