#include <Arduino.h>

#include "GamepadPlus.h"

#define RANGE 30
#define TEST_BUTTON 10

size_t buttonIndex = 0;

CGamepadPlus gamepad_plus(true, true);

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

  gamepad_plus.begin();
  gamepad_plus.setLeftAxisRange(-512, 512);
  gamepad_plus.setRightAxisRange(-512, 512);

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

  auto getMouseDistance = [](const int &axis_value) -> int32_t
  {
    // parameters for reading the joystick:
    static int range = 12;            // output range of X or Y movement
    static int threshold = range / 4; // resting threshold
    static int center = range / 2;    // resting position value

    // map the reading from the analog input range to the output range:
    int reading = map(axis_value, 0, 1023, 0, range);

    // if the output reading is outside from the rest position threshold, use it:
    int distance = reading - center;

    if (abs(distance) < threshold)
    {
      distance = 0;
    }

    // return the distance for this axis:
    return distance;
  };

  gamepad_plus.bind(BindType::LeftAxis, getLeftAxisX, getLeftAxisY);
  gamepad_plus.bind(BindType::RightAxis, getRightAxisX, getRightAxisY);
  // gamepad_plus.bind(BindType::GamepadButton, 1, 11);
  // gamepad_plus.bind(BindType::HatSwitch, 2, 0);
  gamepad_plus.bind(BindType::MouseAxis, getMouseDistance, getMouseDistance);
  gamepad_plus.bind(BindType::MouseActiveButton, 1, 2, 3);

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
          gamepad_plus.press(BindType::HatSwitch, i1 + j * col_pins_count + 1);
          while (!digitalRead(row_pins_num[j]))
            ;
          gamepad_plus.release(BindType::HatSwitch, i1 + j * col_pins_count + 1);
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

  // Serial.print(String("left X:") + String(left_X) + String(" left Y:") + String(left_Y) + String("\n"));
  // gamepad_plus.setLeftAxis(left_X, left_Y);

  int right_X = analogRead(A2);
  int right_Y = analogRead(A3);
  // gamepad_plus.setRightAxis(right_X, right_Y);
  gamepad_plus.setLeftAxis(left_X, left_Y);
  delay(10);
}