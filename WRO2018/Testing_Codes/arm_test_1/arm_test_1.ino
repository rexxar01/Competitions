// Header files
#include <DynamixelSerial2.h>
#include <XYZrobotServo.h>

// Polulu motors (A1-16)
XYZrobotServo base_joint(Serial1, 3);
XYZrobotServo random_servo(Serial1, 1);

// Position coordinates of the arm
/*
 * Joint Description
 *    - Joint base to link 1 : Polulu (ID 3)          - 512
 *    - link 1 to link 2 : Dynamixel (ID 3)           - 512
 *    - link 2 to end effector : Dynamixel (ID 1)     - 512
 */
bool updatePositions = false;
int positions[] = {512, 512, 512, 512};
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
void actuateArmToPositios(bool debug = true) {
  if (updatePositions) {
    Dynamixel.move(1, positions[3]);
    Dynamixel.move(3, positions[2]);
    base_joint.setPosition(positions[1], 40);
    random_servo.setPosition(positions[0], 15);
  } else {
    Serial.println("Arm disabled, give enable command");
  }
  if (debug) {
    String msg = "Arm parameters are [";
    int i;
    for (i = 0; i < 4 - 1; i++) {
      msg.concat(positions[i]);
      msg.concat(", ");
    }
    msg.concat(positions[i]);
    msg.concat("] ");
    Serial.println(msg);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
   * Serial command list
   * [Command][value]
   * Commands:
   *      A -> Position all the joints to [value]
   *      J -> Select joint with [value]
   *          P -> Position the selected joint with [value] (Use J before it in the same command)
   *      U -> Enable positional update
   *      D -> Disable positional update
   */
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'A') {
      int p = Serial.parseInt();
      for (int i = 0; i< 4; i++) {
        positions[i] = p;
      }
    }
    else if (c == 'U') {
      updatePositions = true;
      Serial.println("Update position enabled");
    }
    else if (c == 'D') {
      updatePositions = false;
      Serial.println("Update position disabled");
    }
    else if (c == 'J') {
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

