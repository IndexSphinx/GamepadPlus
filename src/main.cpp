#include <Arduino.h>

#include "GamepadPlus.h"

#define RANGE 30

size_t buttonIndex = 0;

CGamepadPlus gamepadPlus;

size_t row_pins_num[] = {4, 5, 6, 7, 8};
size_t col_pins_num[] = {9, 10, 14, 15, 16};

size_t row_pins_count = sizeof(row_pins_num) / sizeof(row_pins_num[0]);
size_t col_pins_count = sizeof(col_pins_num) / sizeof(col_pins_num[0]);

void setup()
{
  for (size_t i = 0; i < row_pins_count; i++)
    pinMode(row_pins_num[i], INPUT_PULLUP);

  for (size_t i = 0; i < col_pins_count; i++)
    pinMode(col_pins_num[i], OUTPUT);

  gamepadPlus.begin();
  gamepadPlus.setLeftAxisRange(-512, 512);
  gamepadPlus.setRightAxisRange(-512, 512);

  auto getLeftAxisX = [](const int &x) -> int32_t
  {
    if ((x >= 512 - RANGE) && (x <= 512 + RANGE))
    {
      return 0;
    }
    else if (x > 512 + RANGE)
    {
      return -(x - 512);
    }
    else
    {
      return 512 - x;
    }
  };
  auto getLeftAxisY = [](const int &y) -> int32_t
  {
    if ((y >= 512 - RANGE) && (y <= 512 + RANGE))
    {
      return 0;
    }
    else if (y > 512 + RANGE)
    {
      return y - 512;
    }
    else
    {
      return y - 512;
    }
  };

  auto getRightAxisX = [](const int &x) -> int32_t
  {
    if ((x >= 512 - RANGE) && (x <= 512 + RANGE))
    {
      return 0;
    }
    else if (x > 512 + RANGE)
    {
      return -(x - 512);
    }
    else
    {
      return 512 - x;
    }
  };

  auto getRightAxisY = [](const int &y) -> int32_t
  {
    if ((y >= 512 - RANGE) && (y <= 512 + RANGE))
    {
      return 0;
    }
    else if (y > 512 + RANGE)
    {
      return y - 512;
    }
    else
    {
      return y - 512;
    }
  };

  gamepadPlus.bindLeftAxis(getLeftAxisX, getLeftAxisY);
  gamepadPlus.bindRightAxis(getRightAxisX, getRightAxisY);

  Serial.begin(115200);
}

void loop()
{
  auto check_row_pins = [&]() -> bool
  {
    bool ret = false;
    for (size_t i = 0; i < row_pins_count; i++)
    {
      if (!digitalRead(row_pins_num[i]))
      {
        ret = true;
        break;
      }
    }
    return ret;
  };

  auto scan_col_pins = [&]()
  {
    for (size_t i1 = 0; i1 < col_pins_count; i1++)
    {
      for (size_t i2 = 0; i2 < col_pins_count; i2++)
      {
        if (i1 == i2)
          digitalWrite(col_pins_num[i2], LOW);
        else
          digitalWrite(col_pins_num[i2], HIGH);
      }
      for (size_t j = 0; j < row_pins_count; j++)
      {
        if (!digitalRead(row_pins_num[j]))
        {
          Serial.println(i1 + j * col_pins_count + 1);
          // Keyboard.press('x');
          // Joystick.pressButton(GAMEPAD_TEST_BUTTON);
          // Joystick.setHatSwitch(0, 90);

          while (!digitalRead(row_pins_num[j]))
            ;
          // Keyboard.release('x');
          // Joystick.releaseButton(GAMEPAD_TEST_BUTTON);
          // Joystick.setHatSwitch(0, JOYSTICK_HATSWITCH_RELEASE);
        }
      }
    }
  };

  for (size_t i = 0; i < row_pins_count; i++)
    digitalWrite(row_pins_num[i], HIGH);

  for (size_t i = 0; i < col_pins_count; i++)
    digitalWrite(col_pins_num[i], LOW);

  if (check_row_pins())
  {
    delay(10);
    if (check_row_pins())
    {
      scan_col_pins();
    }
  }

  int left_X = analogRead(A0);
  int left_Y = analogRead(A1);

  Serial.print(String("left X:") + String(left_X) + String(" left Y:") + String(left_Y) + String("\n"));
  gamepadPlus.setLeftAxis(left_X, left_Y);

  int right_X = analogRead(A2);
  int right_Y = analogRead(A3);
  gamepadPlus.setRightAxis(right_X, right_Y);

  delay(10);
}