#pragma once

class Game3D
{
public:

	/*
	Game3D Class constructor will initialize the window, OpenGL, and load the game world.
	*/
	Game3D();
	~Game3D();
	/*
	Starts the main game loop.
	*/
	void start();
	void stop();
};

