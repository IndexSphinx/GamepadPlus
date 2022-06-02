#include "GamepadPlus.h"

#include <Joystick.h>
#include <Keyboard.h>
#include <Mouse.h>

GamepadPlus::GamepadPlus()
{
    m_joystick = new Joystick_(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                               JOYSTICK_DEFAULT_BUTTON_COUNT - 16, 1, // Button Count, Hat Switch Count
                               true, true, true,                      // X and Y, but no Z Axis
                               false, false, true,                    // No Rx, Ry, or Rz
                               false, false,                          // No rudder or throttle
                               false, false, false);                  // No accelerator, brake, or steering
}

GamepadPlus::~GamepadPlus()
{
    delete m_joystick;
    m_joystick = nullptr;
}

void GamepadPlus::begin()
{
    if (m_joystick)
    {
        m_joystick->begin();
        m_joystick->setXAxisRange(-512, 512);
        m_joystick->setYAxisRange(-512, 512);
        m_joystick->setZAxisRange(-512, 512);
        m_joystick->setRzAxisRange(-512, 512);
    }
    Keyboard.begin();
}

void GamepadPlus::end()
{
    if (m_joystick)
        m_joystick->end();
    Keyboard.end();
}
