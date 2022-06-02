#include <Arduino.h>
#include <Keyboard.h>
#include <Joystick.h>

#define GAMEPAD_TEST_BUTTON 32

#define RANGE 30

size_t buttonIndex = 0;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   JOYSTICK_DEFAULT_BUTTON_COUNT - 16, 1, // Button Count, Hat Switch Count
                   true, true, true,                      // X and Y, but no Z Axis
                   false, false, true,                    // No Rx, Ry, or Rz
                   false, false,                          // No rudder or throttle
                   false, false, false);                  // No accelerator, brake, or steering

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

  Keyboard.begin();

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-512, 512);
  Joystick.setYAxisRange(-512, 512);
  Joystick.setZAxisRange(-512, 512);
  Joystick.setRzAxisRange(-512, 512);

  Serial.begin(115200);
}

int X, oldX;
int Y, oldY;

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
          Joystick.pressButton(GAMEPAD_TEST_BUTTON);
          Joystick.setHatSwitch(0, 90);

          while (!digitalRead(row_pins_num[j]))
            ;
          // Keyboard.release('x');
          Joystick.releaseButton(GAMEPAD_TEST_BUTTON);
          Joystick.setHatSwitch(0, JOYSTICK_HATSWITCH_RELEASE);
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

  X = analogRead(A0);
  Y = analogRead(A1);

  Serial.print(String("X:") + String(X) + String(" Y:") + String(Y) + String("\n"));
  if ((X != oldX) || (Y != oldY))
  {

    if ((X >= 512 - RANGE) && (X <= 512 + RANGE))
    {
      Joystick.setZAxis(0);
    }
    else if (X > 512 + RANGE)
    {
      Joystick.setZAxis(-(X - 512));
    }
    else
    {
      Joystick.setZAxis(512 - X);
    }

    if ((Y >= 512 - RANGE) && (Y <= 512 + RANGE))
    {
      Joystick.setRzAxis(0);
    }
    else if (Y > 512 + RANGE)
    {
      Joystick.setRzAxis(Y - 512);
    }
    else
    {
      Joystick.setRzAxis(Y - 512);
    }

    oldX = X;
    oldY = Y;
  }

  delay(10);
}