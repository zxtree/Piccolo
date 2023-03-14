#include "runtime/function/global/global_context.h"
#include "game_input_manager.h"
#include "runtime/function/input/input_system.h"

namespace Piccolo
{
    void GameInputManager::initialize()
    {
        m_window_system = g_runtime_global_context.m_window_system.get();

    }

}