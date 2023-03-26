#pragma once

#include "engine.h"
#include "runtime/network/network.h"

namespace Piccolo
{
    class PiccoloGame
    {
    public:
        PiccoloGame();

        virtual ~PiccoloGame();

        void initialize(PiccoloEngine* engine_runtime);

        void clear();
        
        void run();

        PiccoloEngine* m_engine_runtime;
    };
}