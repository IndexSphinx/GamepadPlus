#ifndef __GAMEPADPLUS_H__
#define __GAMEPADPLUS_H__

#define GAMEPAD_BUTTON_X 0
#define GAMEPAD_BUTTON_A 1
#define GAMEPAD_BUTTON_B 2
#define GAMEPAD_BUTTON_Y 3
#define GAMEPAD_BUTTON_L1 4
#define GAMEPAD_BUTTON_R1 5
#define GAMEPAD_BUTTON_L2 6
#define GAMEPAD_BUTTON_R2 7
#define GAMEPAD_BUTTON_SELECT 13
#define GAMEPAD_BUTTON_START 9

class Joystick_;

class GamepadPlus
{
public:
    GamepadPlus();
    virtual ~GamepadPlus();

    void begin();
    void end();

private:
    Joystick_ *m_joystick = nullptr;
};

#endif // __GAMEPADPLUS_H__