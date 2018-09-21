/*
 * Move motor to different positions
 */
#include <XYZrobotServo.h>

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial.begin(115200);
  pinMode(19, INPUT_PULLUP);
}

int pos = 512, id = -1;

void loop() {
  // put your main code here, to run repeatedly:
  XYZrobotServo servoMotor(Serial1, id);
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'P') {
      pos = Serial.parseInt();
      servoMotor.setPosition(pos, 100);
    }
    if (c == 'I') {
      
      servoMotor.setPosition(pos, 100);    
      id = Serial.parseInt();
    }
  }
  Serial.print("Data is : ");
  Serial.println(servoMotor.readStatus().position);
}
