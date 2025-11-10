#include <iostream>

#include "../include/game3D.hpp"

#include "../include/programLogger.hpp"

using::ProgramLogger::log;
//using::ProgramLogger::LogLevel;



//Game3D* myGame3D;

void printMaxVertexAttributes()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
	log("Maximum number of vertex attributes supported: " + std::to_string(nrAttributes));
}

int main()
{
	Game3D* myGame3D = new Game3D();
	printMaxVertexAttributes();
	myGame3D->start();
	myGame3D->stop();
	
    return 0;
}