// Header files
#include <DynamixelSerial2.h>
#include <XYZrobotServo.h>

// Polulu motors (A1-16)
XYZrobotServo base_joint(Serial1, 3);
XYZrobotServo randon_servo(Serial1, 1);

// Position coordinates of the arm
/*
 * Joint Description
 *    - Joint base to link 1 : Polulu (ID 3)
 *    - link 1 to link 2 : Dynamixel (ID 3)
 *    - link 2 to end effector : Dynamixel (ID 1)
 */
int positions[] = {400, 400, 400, 400};
void setup() {
  // Initializing code
  // Dynamixel
  Dynamixel.begin(1000000,2); // Dynamixel selection pin is 2
  // Polulu
  Serial1.begin(115200);
  // User debug
  Serial.begin(115200);
  // Activate pullup for polulu
  pinMode(19, INPUT_PULLUP);
}

// Actuate arm on positions
void actuateArmToPositios() {
  Dynamixel.move(1, positions[3]);
  Dynamixel.move(3, positions[2]);
  base_joint.setPosition(positions[1], 30);
  random_servo.setPosition(positions[0], 15);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
   * Serial command list
   * [Command][value]
   * Commands:
   *      # -> Command mode to all the joints
   *      J -> Select joint with [value]
   *          P -> Position the selected joint with [value]
   */
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '#') {
      int p = Serial.parseInt();
      for (int i = 0; i< 4; i++) {
        positions[i] = p;
      }
    }
    if (c == 'J') {
      int joint_number = Serial.parseInt();
      c = Serial.read();
      if (c == 'P') {
        positions[joint_number] = Serial.parseInt();
        String msg = "Joint ";
        msg.concat(joint_number);
        msg.concat(" mvoed to ");
        msg.concat(positions[joint_number]);
        Serial.println(msg);
      }
    }
  }
  // Set arm to parameters
  actuateArmToPositios();
  delay(100);
}

