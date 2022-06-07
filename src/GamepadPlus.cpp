#include "GamepadPlus.h"

#include <Joystick.h>
#include <Mouse.h>

CGamepadPlus::CGamepadPlus(bool left_axis, bool right_axis, bool hat_switche)
{
    m_joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                               JOYSTICK_DEFAULT_BUTTON_COUNT - 16, hat_switche ? 1 : 0, // Button Count, Hat Switch Count
                               left_axis, left_axis, right_axis,                        // X and Y, but no Z Axis
                               false, false, right_axis,                                // No Rx, Ry, or Rz
                               false, false,                                            // No rudder or throttle
                               false, false, false);                                    // No accelerator, brake, or steering
}

CGamepadPlus::~CGamepadPlus()
{
    delete m_joystick;
    m_joystick = nullptr;
}

void CGamepadPlus::setLeftAxisRange(const int32_t minimum, const int32_t maximum)
{
    m_joystick->setXAxisRange(minimum, maximum);
    m_joystick->setYAxisRange(minimum, maximum);
}

void CGamepadPlus::setRightAxisRange(const int32_t minimum, const int32_t maximum)
{
    m_joystick->setZAxisRange(minimum, maximum);
    m_joystick->setRzAxisRange(minimum, maximum);
}

void CGamepadPlus::bindLeftAxis(const getAxisValue fun_getX, const getAxisValue fun_getY)
{
    m_getLeftAxisX = fun_getX;
    m_getLeftAxisY = fun_getY;
}

void CGamepadPlus::bindRightAxis(const getAxisValue fun_getX, const getAxisValue fun_getY)
{
    m_getRightAxisX = fun_getX;
    m_getRightAxisY = fun_getY;
}

void CGamepadPlus::setLeftAxis(const int &X, const int &Y)
{
    if ((X != m_old_left_X) || (Y != m_old_left_Y))
    {
        if (m_getLeftAxisX && m_joystick)
            m_joystick->setXAxis(m_getLeftAxisX(X));
        if (m_getLeftAxisY && m_joystick)
            m_joystick->setYAxis(m_getLeftAxisY(Y));
        m_old_left_X = X;
        m_old_left_Y = Y;
    }
}

void CGamepadPlus::setRightAxis(const int &X, const int &Y)
{
    if ((X != m_old_right_X) || (Y != m_old_right_Y))
    {
        if (m_getRightAxisX && m_joystick)
            m_joystick->setZAxis(m_getRightAxisX(X));
        if (m_getRightAxisY && m_joystick)
            m_joystick->setRzAxis(m_getRightAxisY(Y));
        m_old_right_X = X;
        m_old_right_Y = Y;
    }
}

void CGamepadPlus::begin()
{
    if (m_joystick)
    {
        m_joystick->begin();
    }
    Keyboard.begin();
}

void CGamepadPlus::end()
{
    if (m_joystick)
        m_joystick->end();
    Keyboard.end();
}
