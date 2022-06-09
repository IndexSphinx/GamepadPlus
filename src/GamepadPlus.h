#ifndef __GAMEPADPLUS_H__
#define __GAMEPADPLUS_H__

#include <stdint.h>
#include <vector>
#include <Keyboard.h>

#define GAMEPAD_BUTTON_X 0
#define GAMEPAD_BUTTON_A 1
#define GAMEPAD_BUTTON_B 2
#define GAMEPAD_BUTTON_Y 3
#define GAMEPAD_BUTTON_L1 4
#define GAMEPAD_BUTTON_R1 5
#define GAMEPAD_BUTTON_L2 6
#define GAMEPAD_BUTTON_R2 7
#define GAMEPAD_BUTTON_L 10
#define GAMEPAD_BUTTON_R 11
#define GAMEPAD_BUTTON_SELECT 13
#define GAMEPAD_BUTTON_START 9

class Joystick_;

using funGetAxisValue = int32_t (*)(const int &);

enum class BindType
{
    None,
    LeftAxis,
    RightAxis,
    GamepadButton,
    HatSwitch,
    KeyboardButton,
};

class Button
{
public:
    Button(BindType bind_type, uint8_t index, uint8_t value)
        : m_bind_type(bind_type), m_index(index), m_value(value){};
    ~Button() = default;
    BindType getBindType() { return m_bind_type; }
    uint8_t getIndex() { return m_index; }
    uint8_t getValue() { return m_value; }

private:
    BindType m_bind_type = BindType::None;
    uint8_t m_index = 0;
    uint8_t m_value = 0;
};

class CGamepadPlus
{
public:
    CGamepadPlus(bool left_axis = true, bool right_axis = true, bool hat_switch = true);
    virtual ~CGamepadPlus();

    void setLeftAxisRange(const int32_t minimum, const int32_t maximum);
    void setRightAxisRange(const int32_t minimum, const int32_t maximum);

    void bind(const BindType bind_type, const funGetAxisValue fun_getX, const funGetAxisValue fun_getY);
    void bind(const BindType bind_type, const int index, const int button_value);

    void setLeftAxis(const int &X, const int &Y);
    void setRightAxis(const int &X, const int &Y);
    void press(const BindType &bind_type, const uint8_t &index);
    void release(const BindType &bind_type, const uint8_t index);

    void begin();
    void end();

private:
    void bindLeftAxis(const funGetAxisValue fun_getX, const funGetAxisValue fun_getY);
    void bindRightAxis(const funGetAxisValue fun_getX, const funGetAxisValue fun_getY);
    void bindGamepadButton(const int index, const int value);
    void bindHatSwitchButton(const int index, const int value);
    void bindKeyboardButton(const int index, const int value);

private:
    Joystick_ *m_joystick = nullptr;

    funGetAxisValue m_get_left_axis_X = nullptr;
    funGetAxisValue m_get_left_axis_Y = nullptr;
    funGetAxisValue m_get_right_axis_X = nullptr;
    funGetAxisValue m_get_right_axis_Y = nullptr;
    std::vector<Button> m_buttons;
    int m_old_left_X = 0;
    int m_old_left_Y = 0;
    int m_old_right_X = 0;
    int m_old_right_Y = 0;
};

#endif // __GAMEPADPLUS_H__