#include<FourSBase.h>


int S4BASE_PINOUT_PWMs[] = {8, 8, 7, 6};
int S4BASE_PINOUT_DIRs[] = {47, 47, 49, 45};
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
