#pragma once

namespace Piccolo
{
    enum class InputCommand : unsigned int
    {
        UP      = 1 << 0,
        DOWN    = 1 << 1,
        LEFT    = 1 << 2,
        RIGHT   = 1 << 3,

        BUTTON_A    = 1 << 4,
        BUTTON_B    = 1 << 5,
        BUTTON_C    = 1 << 6,
        BUTTON_D    = 1 << 7,

        TRIGGER_LT  = 1 << 8,
        TRIGGER_LB  = 1 << 9,
        TRIGGER_RT  = 1 << 10,
        TRIGGER_RB  = 1 << 11,

        START       = 1 << 12,
        OPTION      = 1 << 13,
        ESCAPE      = 1 << 14,
    };

    class GameInputManager
    {
    public:
        void initialize();

        void tick(float delta_time);

        class WindowSystem* m_window_system {nullptr};
    };
}