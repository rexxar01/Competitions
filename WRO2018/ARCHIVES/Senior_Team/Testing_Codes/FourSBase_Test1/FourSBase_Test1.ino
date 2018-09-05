#include<FourSBase.h>

int S4BASE_PINOUT_DIRs[] = {53, 49, 47, 51};
int S4BASE_PINOUT_PWMs[] = {10, 7, 8, 9};
bool S4BASE_PIN_REVs[] = {true, false, false, true};
FourSBase robotBase;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  robotBase.Initialize("FWS_1", &Serial, 0);
  robotBase.AddPins(S4BASE_PINOUT_PWMs, S4BASE_PINOUT_DIRs, S4BASE_PIN_REVs);
  Serial.println("Going into loop...");
}

int pwm = 0, angle = 0;

void loop() {
  // put your main code here, to run repeatedly:
  // Serial communication : send P<PWM>A<ANGLE> to move at PWM at an angle w.r.t forward. Use D<ms> to cause a delay
  if(Serial.available()) {
    char c = Serial.read();
    if (c == 'P') {
      // PWM
      Serial.println("'P' (PWM) received");
      int val = Serial.parseInt();
      Serial.print(val);
      Serial.println(" received");
      pwm = val;
    }
    else if (c == 'D') {
      // Delay (Do nothing)
      Serial.println("Delay started");
      int val = Serial.parseInt();
      delay(val);
      Serial.println("Delay ended");
    }
    else if (c == 'A') {
      // Angle
      Serial.println("'A' (angle) received");
      int val = Serial.parseInt();
      Serial.print(val);
      Serial.println(" received");
      angle = val;
    }
  }
  // Actuate bot
  robotBase.Move(pwm, angle);
}
