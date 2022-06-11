#include "GamepadPlus.h"

#include <Joystick.h>
#include <Mouse.h>

CGamepadPlus::CGamepadPlus(bool enable_keyboard = false, bool enable_mouse = false, bool left_axis, bool right_axis, bool hat_switche)
    : m_enable_keyboard(enable_keyboard), m_enable_mouse(enable_mouse)
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

void CGamepadPlus::bind(const BindType bind_type, const funGetAxisValue fun_getX, const funGetAxisValue fun_getY)
{
    switch (bind_type)
    {
    case BindType::LeftAxis:
        bindLeftAxis(fun_getX, fun_getY);
        break;
    case BindType::RightAxis:
        bindRightAxis(fun_getX, fun_getY);
        break;
    case BindType::MouseAxis:
        bindMouseAxis(fun_getX, fun_getY);
        break;
    default:
        break;
    }
}

void CGamepadPlus::bind(const BindType bind_type, const int index, const int button_value)
{
    switch (bind_type)
    {
    case BindType::GamepadButton:
        bindGamepadButton(index, button_value);
        break;
    case BindType::HatSwitch:
        bindHatSwitchButton(index, button_value);
        break;
    case BindType::KeyboardButton:
        if (m_enable_keyboard)
            bindKeyboardButton(index, button_value);
        break;
    default:
        break;
    }
}

void CGamepadPlus::bind(const BindType bind_type, const int active_mouse_index, const int mouse_left_index, const int mouse_right_index,
                        const bool enable_left_axis = true, const bool enable_right_axis = false)
{
    if (m_enable_mouse)
    {
        for (auto button : m_buttons)
        {
            if (button.getIndex() == active_mouse_index)
                return;
        }
        m_buttons.push_back(Button(BindType::MouseActiveButton, active_mouse_index, 0));

        switch (bind_type)
        {
        case BindType::MouseActiveButton:
        {
            m_mouse_active_index = active_mouse_index;
            m_mouse_left_index = mouse_left_index;
            m_mouse_right_index = mouse_right_index;
            m_left_mouse_mode = enable_left_axis;
            m_right_mouse_mode = enable_right_axis;
        }
        break;
        default:
            break;
        }
    }
}

void CGamepadPlus::setLeftAxis(const int &X, const int &Y)
{
    if (m_mouse_is_active && m_left_mouse_mode)
    {
        Mouse.move(m_get_mouse_left_distance(X), m_get_mouse_left_distance(Y), 0);
    }
    else
    {
        if ((X != m_old_left_X) || (Y != m_old_left_Y))
        {
            if (m_get_left_axis_X && m_joystick)
                m_joystick->setXAxis(m_get_left_axis_X(X));
            if (m_get_left_axis_Y && m_joystick)
                m_joystick->setYAxis(m_get_left_axis_Y(Y));
            m_old_left_X = X;
            m_old_left_Y = Y;
        }
    }
}

void CGamepadPlus::setRightAxis(const int &X, const int &Y)
{
    if (m_mouse_is_active && m_right_mouse_mode)
    {
        Mouse.move(m_get_mouse_left_distance(X), m_get_mouse_right_distance(Y), 0);
    }
    else
    {
        if ((X != m_old_right_X) || (Y != m_old_right_Y))
        {
            if (m_get_right_axis_X && m_joystick)
                m_joystick->setZAxis(m_get_right_axis_X(X));
            if (m_get_right_axis_Y && m_joystick)
                m_joystick->setRzAxis(m_get_right_axis_Y(Y));
            m_old_right_X = X;
            m_old_right_Y = Y;
        }
    }
}

void CGamepadPlus::press(const BindType &bind_type, const uint8_t &index)
{
    for (auto button : m_buttons)
    {
        // Serial.print(String("getBindType:") + String((int)button.getBindType()) + String(" getIndex:") + String(button.getIndex()) + String(" getValue:") + String(button.getValue()) + String("\n"));

        if (button.getIndex() == index)
        {
            switch (button.getBindType())
            {
            case BindType::GamepadButton:
                if (m_joystick)
                    m_joystick->pressButton(button.getValue());
                break;
            case BindType::HatSwitch:
                if (m_joystick)
                    m_joystick->setHatSwitch(0, button.getValue());
                break;
            case BindType::KeyboardButton:
                Keyboard.press(button.getValue());
                break;
            case BindType::MouseActiveButton:
                m_mouse_is_active = !m_mouse_is_active;
                return;
            default:
                break;
            }
        }

        if (m_mouse_is_active && m_mouse_left_index == index)
        {
            Mouse.press(MOUSE_LEFT);
        }
        else if (m_mouse_is_active && m_mouse_right_index == index)
        {
            Mouse.press(MOUSE_RIGHT);
        }
    }
}

void CGamepadPlus::release(const BindType &bind_type, const uint8_t index)
{
    for (auto button : m_buttons)
    {
        if (button.getIndex() == index)
        {
            switch (button.getBindType())
            {
            case BindType::GamepadButton:
                if (m_joystick)
                    m_joystick->releaseButton(button.getValue());
                break;
            case BindType::HatSwitch:
                if (m_joystick)
                    m_joystick->setHatSwitch(0, JOYSTICK_HATSWITCH_RELEASE);
                break;
            case BindType::KeyboardButton:
                Keyboard.release(button.getValue());
                break;
            default:
                break;
            }
        }
    }
    if (m_mouse_is_active && m_mouse_left_index == index)
    {
        Mouse.release(MOUSE_LEFT);
    }
    else if (m_mouse_is_active && m_mouse_right_index == index)
    {
        Mouse.release(MOUSE_RIGHT);
    }
}

void CGamepadPlus::begin()
{
    if (m_joystick)
    {
        m_joystick->begin();
    }
    Keyboard.begin();
    Mouse.begin();
}

void CGamepadPlus::end()
{
    if (m_joystick)
        m_joystick->end();
    Keyboard.end();
    Mouse.end();
}

void CGamepadPlus::bindLeftAxis(const funGetAxisValue fun_getX, const funGetAxisValue fun_getY)
{
    m_get_left_axis_X = fun_getX;
    m_get_left_axis_Y = fun_getY;
}

void CGamepadPlus::bindRightAxis(const funGetAxisValue fun_getX, const funGetAxisValue fun_getY)
{
    m_get_right_axis_X = fun_getX;
    m_get_right_axis_Y = fun_getY;
}

void CGamepadPlus::bindMouseAxis(const funGetAxisValue fun_getX, const funGetAxisValue fun_getY)
{
    m_get_mouse_left_distance = fun_getX;
    m_get_mouse_right_distance = fun_getY;
}

void CGamepadPlus::bindGamepadButton(const int index, const int value)
{
    for (auto button : m_buttons)
    {
        if (button.getIndex() == index)
            return;
    }
    m_buttons.push_back(Button(BindType::GamepadButton, index, value));
}

void CGamepadPlus::bindHatSwitchButton(const int index, const int value)
{
    for (auto button : m_buttons)
    {
        if (button.getIndex() == index)
            return;
    }
    m_buttons.push_back(Button(BindType::HatSwitch, index, value));
}

void CGamepadPlus::bindKeyboardButton(const int index, const int value)
{
    for (auto button : m_buttons)
    {
        if (button.getIndex() == index)
            return;
    }
    m_buttons.push_back(Button(BindType::KeyboardButton, index, value));
}
