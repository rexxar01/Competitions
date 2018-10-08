#include <BotBase.h>
#include <D45Base.h>

int D45BASE_DIRs[] = {47, 53, 51, 49};
int D45BASE_PWMs[] = {8, 10, 9, 7};
bool D45BASE_REVs[] = {false, false, true, true};

D45Base robotBase;

void setup()
{
  Serial2.begin(38400);
  Serial.begin(38400);
  robotBase.Initialize("FWS_1", &Serial, 2);
  robotBase.AssignPins(D45BASE_PWMs, D45BASE_DIRs, D45BASE_REVs);
  Serial.println("Going into loop ....");
  robotBase.Move(0, 0);
}

int pwm = 50;

void loop()
{
  while (Serial2.available())
  {
    char ch = Serial2.read();
    Serial.println(ch);
    switch (ch)
    {
      case 'F': robotBase.Move(pwm, 90);
        break;
      case 'B': robotBase.Move(pwm, 270);
        break;
      case 'R': robotBase.Move(pwm, 0);
        break;
      case 'L': robotBase.Move(pwm, 180);
        break;
      case 'H': robotBase.Move(pwm, 225);
        break;
      case 'G': robotBase.Move(pwm, 135);
        break;
      case 'I': robotBase.Move(pwm, 45);
        break;
      case 'J': robotBase.Move(pwm, 315);
        break;
      default:  robotBase.KillMotors();
    }
  }
}
