#pragma once

#include "runtime/core/math/math.h"
#include <GLFW/glfw3.h>

namespace Piccolo
{
    enum class GameCommand : unsigned int
    {
        NONE            = 0,
        FORWARD         = 1, // 1 << 1
        BACKWARD        = 2, // 1 << 2
        LEFT            = 3,
        RIGHT           = 4,
        
        BUTTON_A        = 5,
        BUTTON_B        = 6,
        BUTTON_C        = 7,
        BUTTON_D        = 8,

        TRIGGER_LT      = 9,
        TRIGGER_LB      = 10,
        TRIGGER_RT      = 11,
        TRIGGER_RB      = 12,

        SELECT          = 13,
        OPTIONS         = 14,
        START           = 15,
        ESCAPE          = 16,

        INVALID         = 17
        //INVALID         = (unsigned int) (1 << 31) // lost focus
    };

    extern unsigned int k_complement_control_command;

    class InputSystem
    {

    public:
        void onKey(int key, int scancode, int action, int mods);
        void onCursorPos(double current_cursor_x, double current_cursor_y);

        void initialize();
        void tick();
        void clear();

        int m_cursor_delta_x {0};
        int m_cursor_delta_y {0};

        Radian m_cursor_delta_yaw {0};
        Radian m_cursor_delta_pitch {0};

        void         resetGameCommand() { m_game_command = 0; }
        unsigned int getGameCommand() const { return m_game_command; }

    private:
        void onKeyInGameMode(int key, int scancode, int action, int mods);

        void calculateCursorDeltaAngles();

        void initKeymap();
        void initGamepadMap();

        unsigned int m_game_command {0};

        int m_last_cursor_x {0};
        int m_last_cursor_y {0};

        GameCommand m_keymap[GLFW_KEY_LAST] = {GameCommand::NONE};
        GameCommand m_gamepad_map[GLFW_GAMEPAD_BUTTON_LAST] = {GameCommand::NONE};
    };
} // namespace Piccolo
