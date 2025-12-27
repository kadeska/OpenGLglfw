#include <iostream>

#include <glad/glad.h>

#include "../include/game3D.hpp"
#include "../include/programLogger.hpp"
using::ProgramLogger::log;
using::ProgramLogger::LogLevel;


//#include <assimp/Importer.hpp>
//Assimp::Importer importer;



//Game3D* myGame3D;

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

int main(int argc, char* argv[])
{


	Game3D* myGame3D = new Game3D();
	printMaxVertexAttributes();
	myGame3D->start();
	myGame3D->stop();
	
    return 0;
}



//int loadFreeType() 
//{
//	log("Loading FreeType...");
//	FT_Library ft;
//	if (FT_Init_FreeType(&ft))
//	{
//		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//		return -1;
//	}
//
//	//FT_Face face;
//	//if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
//	//{
//	//	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//	//	return -1;
//	//}
//
//	//FT_Set_Pixel_Sizes(face, 0, 48);
//}
