#include <vector>


#include "world.hpp"
#include "../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

World::World()
{
	log("World Constructor", LogLevel::DEBUG_V);
	//worldData = { "path", };
}

void World::update()
{
}
