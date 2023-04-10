#pragma once

#include "piccolodef.h"
#include "engine.h"
#include "runtime/network/network.h"

PCL_GAME_NS_OPEN

class PiccoloGame {
public:
    PiccoloGame();

    virtual ~PiccoloGame();

    void initialize(PiccoloEngine* engine_runtime);

    void clear();
    
    void run();

    PiccoloEngine* m_engine_runtime;
};

PCL_GAME_NS_CLOSE