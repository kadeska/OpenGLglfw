
#include "../include/programLogger.hpp"

#include "../include/entity/entityPlayer.hpp"

using ProgramLogger::log;
using ProgramLogger::LogLevel;

EntityPlayer::EntityPlayer(glm::vec3 _playerLocation)
{
	log("EntityPlayer constructor");
}
