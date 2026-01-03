#include <iostream>

#include <glad/glad.h>

#include "game3D.hpp"
#include "misc/programLogger.hpp"
using::ProgramLogger::log;
using::ProgramLogger::LogLevel;

#include "utils/fileManager.hpp"


std::string outFile;


//static void bar(int a) 
//{
//	std::cout << "Value: " << a << std::endl;
//}


// Must be called after loading OpenGL functions.
// In this case, after the window loads OpenGL, 
// which takes place in the constructor of Game3D.
void printMaxVertexAttributes()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
	log("Maximum number of vertex attributes supported: " + std::to_string(nrAttributes));
}

bool readConfig() 
{
	
	FileManager::DefaultConfig defaultConfig; // create instance of your default config

	// Ensure consistent folder usage
	const std::string filename = "testData.cfg";

	if (FileManager::readDataFromFile(filename, outFile) == 0) // 0 = success
	{
		log("File exists, not creating new one.", LogLevel::DEBUG);
		return true;
	}

	log("Config File was not found, creating a new default config file now...");
	if (FileManager::saveDataToFile("testData.cfg", defaultConfig.data) == 0)
	{
		log("Config File created successfully", LogLevel::WARNING);
		return false;
	}
	else
	{
		log("Failed to create Config File.", ProgramLogger::LogLevel::ERROR);
		return false;
	}
	return false;
}


int main(int argc, char* argv[])
{
	log("Main entry", LogLevel::DEBUG);

	FileManager::initializeFileManager();
	
	if (!readConfig()) 
	{
		log("Config file either didnt exist or there was some other error.", LogLevel::DEBUG);
		// check if the outFile contains data
		if (!outFile.empty()) 
		{
			// safe to assume file didnt exist and a new one was created. Go ahead and process the data.
			FileManager::processConfig(outFile);
		}
	}
	else  // config read sucessfuly, move on with processing
	{
		FileManager::processConfig(outFile);
	}
	

	log("Config File read successfuly.");
	


	// initialize game
	Game3D* game = new Game3D();
	//printMaxVertexAttributes();
	game->start();
	game->stop();


	// I do the following below to keep the console window open until the user presses any button.
	std::cout << '\n' << "Game quit in State: " + std::to_string(static_cast<int>(game->gameStateManager.getState())) << '\n';
	// if the game quit during loading, RIP any save data that might be corupted 
	std::cout << '\n' << "Game has quit, press ENTER to close console... " << '\n';

	std::cin.get(); // wait for user input before closing console
	
    return 0;
}