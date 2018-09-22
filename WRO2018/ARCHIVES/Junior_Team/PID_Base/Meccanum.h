#include "Arduino.h"
class Meccanum
{
    int dir_pins[4], pwm_pins[4];
    int Pwm[4];
    bool Dir[4];

  public:
    void Init();
    void AssignMeccanum(int [], int []);
    void ModPwm(int, int);
    void AttachPwm(int, int);
    void Kill();
    void MoveMotor();
};

void Meccanum :: AssignMeccanum(int Dir_Pins[], int Pwm_Pins[])
{
  int i;
  for (i = 0; i < 4; i++)
  {
    this->dir_pins[i] = Dir_Pins[i];
    this->pwm_pins[i] = Pwm_Pins[i];
    Dir[i] = LOW;
    Pwm[i] = 0;
  }
}

void Meccanum :: Init()
{
  int i;
  for (i = 0; i < 4; i++)
  {
    pinMode(dir_pins[i], OUTPUT);
    pinMode(pwm_pins[i], OUTPUT);
  }
}

void Meccanum :: AttachPwm(int pin, int mag)
{
  Dir[pin] = mag > 0 ? HIGH : LOW;
  Pwm[pin] = fabs(mag);
}

void Meccanum :: Kill()
{
  for (int i = 0; i < 4; i++)
  {
    AttachPwm(i, 0);
  }
  MoveMotor();
}

void Meccanum :: MoveMotor()
{
  int i;
  for (i = 0; i < 4; i++)
  {
    digitalWrite(dir_pins[i], Dir[i]);
    if (Pwm[i] > 255)
    {
      Pwm[i] = 255;
    }
    if(Dir[i]==HIGH)
    Serial.print(Pwm[i]);
    else
    Serial.print(-Pwm[i]);
    Serial.print("\t");
    analogWrite(pwm_pins[i], Pwm[i]);
  }
  Serial.println();
}

void Meccanum :: ModPwm(int motor_pin, int mag)
{
  int temp = (Dir[motor_pin]==HIGH) ? Pwm[motor_pin] : -Pwm[motor_pin];
  temp += mag;
  AttachPwm(motor_pin, temp);
}
