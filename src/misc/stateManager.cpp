#include "stateManager.hpp"
#include "../misc/programLogger.hpp"

using ProgramLogger::log;
using ProgramLogger::LogLevel;
using namespace StateManager;

void GameStateManager::setState(GameState newState)
{
    log("changing game state", LogLevel::DEBUG);
    auto newIndex = (int)newState;
    if (newIndex < 0 || newIndex >= (int)GameState::COUNT) {
        log("Invalid GameState enum value", LogLevel::STATE);
        return;
    }

    if (currentState == newState) {
        log(
            std::string("Ignored state change: already in ") + toString(currentState), LogLevel::STATE
        );
        return;
    }

    if (!isValidTransition(currentState, newState)) {
        log(std::string("Invalid state transition: ") + toString(currentState) + " -> " + toString(newState), LogLevel::STATE);
        return;
    }

    log(std::string("State exit: ") + toString(currentState), LogLevel::STATE);

    auto exitCb = exitCallbacks[static_cast<int>(currentState)];
    if (exitCb) exitCb();

    previousState = currentState;
    currentState = newState;

    log(std::string("State transition: ") + toString(previousState) + " -> " + toString(currentState), LogLevel::STATE);


    log(std::string("State enter: ") + toString(currentState) + " (from " + toString(previousState) + ")", LogLevel::STATE);

    auto enterCb = enterCallbacks[(int)currentState];
    if (enterCb) enterCb();
}


bool GameStateManager::isValidTransition(GameState from, GameState to)
{
    switch (from) {
    case GameState::NONE:
        return to == GameState::MAIN_MENU;
    case GameState::LOADING:
        return to == GameState::PLAYING;
    case GameState::PLAYING:
        return to == GameState::PAUSED || to == GameState::GAME_OVER;
    case GameState::PAUSED:
        return to == GameState::PLAYING || to == GameState::MAIN_MENU;
	case GameState::GAME_OVER:
		return to == GameState::MAIN_MENU;
    case GameState::MAIN_MENU:
		return to == GameState::LOADING || to == GameState::NONE;
    default:
        return false;
    }
}

void GameStateManager::onEnter(GameState state, StateCallback callback)
{
    log("Registering onEnter CB", LogLevel::DEBUG);
    int index = (int)state;
    if (index < 0 || index >= (int)GameState::COUNT)
        return;

    enterCallbacks[index] = callback;
}


void GameStateManager::onExit(GameState state, StateCallback callback)
{
    log("Registering onExit CB", LogLevel::DEBUG);
    int index = (int)state;
    if (index < 0 || index >= (int)GameState::COUNT)
        return;

    exitCallbacks[index] = callback;
}

GameState GameStateManager::getState() const
{
    return currentState;
}

GameState GameStateManager::getPreviousState() const
{
    return previousState;
}

bool GameStateManager::is(GameState state) const
{
    return currentState == state;
}


const char* StateManager::toString(GameState state)
{
    switch (state) {
    case GameState::NONE:       return "NONE";
    case GameState::LOADING:    return "LOADING";
    case GameState::MAIN_MENU:  return "MAIN_MENU";
    case GameState::PLAYING:    return "PLAYING";
    case GameState::PAUSED:     return "PAUSED";
    case GameState::GAME_OVER:  return "GAME_OVER";
    case GameState::COUNT: return "COUNT";
    default:                    return "UNKNOWN";
    }
}