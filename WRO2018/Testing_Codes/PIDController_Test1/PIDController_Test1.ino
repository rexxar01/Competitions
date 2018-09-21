#include<PIDController.h>

PIDController c1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initializing...");
  c1.InitializeDebugger("CONT_1", &Serial, 1);
  c1.assignPIDParameters(1.0, 0.01, 0.1);
  Serial.println("Done...");
}

double setPoint = 512;

void loop() {
  // put your main code here, to run repeatedly:
  int currentValue = analogRead(A0);
  String msg = "Current Value : ";
  msg.concat(currentValue);
  msg.concat(" Set Point : ");
  msg.concat(setPoint);
  double correction = c1.retError(setPoint, currentValue);
  msg.concat(" Correction : ");
  msg.concat(correction);
  Serial.println(msg);
  delay(100);
}
