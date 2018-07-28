#include <S4Base.h>

int S4BASE_PINOUT_PWMs[] = {5, 8, 7, 6};
int S4BASE_PINOUT_DIRs[] = {43, 47, 49, 45};
bool S4BASE_PIN_REVs[] = {true, false, false, true};

S4Base robotBase;
void setup() {
  // put your setup code here, to run once:
  robotBase.Initialize("S4Base_1", &Serial, 3); // Default baud rate is 57600
  robotBase.AddMotorDriverPins(S4BASE_PINOUT_PWMs, S4BASE_PINOUT_DIRs, S4BASE_PIN_REVs);
}
int pwm = 0;
int angle = 0;

void loop() {
  robotBase.Move(pwm, angle);
  // put your main code here, to run repeatedly:
  while(Serial.available()) {
    char c = Serial.read();
    if (c == 'P') {
      Serial.println("'P' (PWM) received");
      int val = Serial.parseInt();
      Serial.print(val);
      Serial.println(" received");
      pwm = val;
    }
    else if (c == 'A') {
      Serial.println("'A' (angle) received");
      int val = Serial.parseInt();
      Serial.print(val);
      Serial.println(" received");
      angle = val;
    }
  }
}
