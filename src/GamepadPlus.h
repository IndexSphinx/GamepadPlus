#ifndef __GAMEPADPLUS_H__
#define __GAMEPADPLUS_H__

#include <stdint.h>
#include <Keyboard.h>

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

using getAxisValue = int32_t (*)(const int &);

class CGamepadPlus
{
public:
    CGamepadPlus(bool left_axis = true, bool right_axis = true, bool hat_switch = true);
    virtual ~CGamepadPlus();

    void setLeftAxisRange(const int32_t minimum, const int32_t maximum);
    void setRightAxisRange(const int32_t minimum, const int32_t maximum);

    void bindLeftAxis(const getAxisValue fun_getX, const getAxisValue fun_getY);
    void bindRightAxis(const getAxisValue fun_getX, const getAxisValue fun_getY);

    void setLeftAxis(const int &X, const int &Y);
    void setRightAxis(const int &X, const int &Y);

    void begin();
    void end();

private:
    Joystick_ *m_joystick = nullptr;

    getAxisValue m_getLeftAxisX = nullptr;
    getAxisValue m_getLeftAxisY = nullptr;
    getAxisValue m_getRightAxisX = nullptr;
    getAxisValue m_getRightAxisY = nullptr;
    int m_old_left_X = 0;
    int m_old_left_Y = 0;
    int m_old_right_X = 0;
    int m_old_right_Y = 0;
};

#endif // __GAMEPADPLUS_H__