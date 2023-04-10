#include "game_mode.h"
#include "runtime/core/base/log.h"

PCL_NAMESPACE_USE

GameMode::GameMode(WorldManager* world) {
    m_world = world;
}

GameMode::~GameMode() {
    m_world = nullptr;
}

void GameMode::initialize() {
    LOG_INFO("game mode initialize");
    b_is_initialized = true;
}

void GameMode::pause() {
}

void GameMode::resume() {
}

void GameMode::tick(float delta_time) {
    if(!b_is_initialized) {
        initialize();
    }

    LOG_INFO("game mode tick: {}", delta_time);    
}

void GameMode::stop() {
    b_is_initialized = false;
}