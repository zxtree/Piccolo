#include "runtime/function/input/input_system.h"

#include "core/base/macro.h"

#include "runtime/engine.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

namespace Piccolo
{
    unsigned int k_complement_control_command = 0xFFFFFFFF;

    void InputSystem::onKey(int key, int scancode, int action, int mods)
    {
        if (!g_is_editor_mode)
        {
            onKeyInGameMode(key, scancode, action, mods);
        }
    }

    void InputSystem::initKeymap()
    {
        m_keymap[GLFW_KEY_W]            = GameCommand::FORWARD;
        m_keymap[GLFW_KEY_A]            = GameCommand::LEFT;
        m_keymap[GLFW_KEY_S]            = GameCommand::BACKWARD;
        m_keymap[GLFW_KEY_D]            = GameCommand::RIGHT;

        m_keymap[GLFW_KEY_J]            = GameCommand::BUTTON_A;
        m_keymap[GLFW_KEY_K]            = GameCommand::BUTTON_B;
        m_keymap[GLFW_KEY_U]            = GameCommand::BUTTON_C;
        m_keymap[GLFW_KEY_I]            = GameCommand::BUTTON_D;

        m_keymap[GLFW_KEY_ESCAPE]       = GameCommand::ESCAPE;
    }

    void InputSystem::initGamepadMap()
    {
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_A]                = GameCommand::BUTTON_A;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_B]                = GameCommand::BUTTON_B;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_X]                = GameCommand::BUTTON_C;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_Y]                = GameCommand::BUTTON_D;

        m_gamepad_map[GLFW_GAMEPAD_BUTTON_DPAD_UP]          = GameCommand::FORWARD;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]        = GameCommand::BACKWARD;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]        = GameCommand::LEFT;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]       = GameCommand::RIGHT;

        m_gamepad_map[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]       = GameCommand::TRIGGER_LT;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER]      = GameCommand::TRIGGER_LB;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]      = GameCommand::TRIGGER_RT;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]     = GameCommand::TRIGGER_RB;

        m_gamepad_map[GLFW_GAMEPAD_BUTTON_START]            = GameCommand::START;
        m_gamepad_map[GLFW_GAMEPAD_BUTTON_BACK]             = GameCommand::ESCAPE;
    }

    void InputSystem::onKeyInGameMode(int key, int scancode, int action, int mods)
    {
        //m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::jump);

        GameCommand cmd = m_keymap[key];
        LOG_DEBUG("on key game command: {}", cmd);
        if (action == GLFW_PRESS)
        {
            m_game_command |= (1 << (unsigned int) cmd);
        }
        else if (action == GLFW_RELEASE)
        {
            m_game_command &= (1 << (unsigned int) cmd);
        }
    }

    void InputSystem::onCursorPos(double current_cursor_x, double current_cursor_y)
    {
        if (g_runtime_global_context.m_window_system->getFocusMode())
        {
            m_cursor_delta_x = m_last_cursor_x - current_cursor_x;
            m_cursor_delta_y = m_last_cursor_y - current_cursor_y;
        }
        m_last_cursor_x = current_cursor_x;
        m_last_cursor_y = current_cursor_y;
    }

    void InputSystem::clear()
    {
        m_cursor_delta_x = 0;
        m_cursor_delta_y = 0;
    }

    void InputSystem::calculateCursorDeltaAngles()
    {
        std::array<int, 2> window_size = g_runtime_global_context.m_window_system->getWindowSize();

        if (window_size[0] < 1 || window_size[1] < 1)
        {
            return;
        }

        std::shared_ptr<RenderCamera> render_camera = g_runtime_global_context.m_render_system->getRenderCamera();
        const Vector2&                fov           = render_camera->getFOV();

        Radian cursor_delta_x(Math::degreesToRadians(m_cursor_delta_x));
        Radian cursor_delta_y(Math::degreesToRadians(m_cursor_delta_y));

        m_cursor_delta_yaw   = (cursor_delta_x / (float)window_size[0]) * fov.x;
        m_cursor_delta_pitch = -(cursor_delta_y / (float)window_size[1]) * fov.y;
    }

    void InputSystem::initialize()
    {
        initKeymap();
        initGamepadMap();
        
        std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;
        ASSERT(window_system);

        window_system->registerOnKeyFunc(std::bind(&InputSystem::onKey,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3,
                                                   std::placeholders::_4));
        window_system->registerOnCursorPosFunc(
            std::bind(&InputSystem::onCursorPos, this, std::placeholders::_1, std::placeholders::_2));
    }

    void InputSystem::tick()
    {
        calculateCursorDeltaAngles();
        clear();

        std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;
        // if (window_system->getFocusMode())
        // {
        //     m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::invalid);
        // }
        // else
        // {
        //     m_game_command |= (unsigned int)GameCommand::invalid;
        // }
    }
} // namespace Piccolo
