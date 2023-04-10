#pragma once

#include "piccolodef.h"

PCL_NAMESPACE_OPEN

class WorldManager;

class GameMode {
public:
    WorldManager* m_world {nullptr};
    bool b_is_initialized {false};
    
    GameMode(WorldManager* world);
    virtual ~GameMode();

    virtual void initialize();
    virtual void pause();
    virtual void resume();
    virtual void tick(float delta_time);
    virtual void stop();
};

PCL_NAMESPACE_CLOSE