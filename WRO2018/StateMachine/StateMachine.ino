#include "Arduino.h"
#include <XYZrobotServo.h>
#include <DynamixelSerial1.h>
#include <Servo.h>

/*##########################################

  Constants -

  ##########################################*/

#define TimeConstraint 150000
#define TrigPin 50
#define EchoPin 52
#define Limit_1 12
#define Limit_2 25
#define SM_1_Pin 4
#define SM_2_Pin 27
#define SM_1_Lower 150
#define SM_1_Upper 90
#define SM_2_Lower 0
#define SM_2_Upper 60
#define IndicatorLED 3
#define DynSelectPin 22
#define ArmServoPin 11
#define RX3Pin 15
#define PololuID 3
#define DynaxID1 1
#define DynaxID2 3

int BasePWM = 150;
int BaseDirPins[] = {53, 49, 47, 51};
int BasePwmPins[] = {10, 7, 8, 9};
XYZrobotServo BasePololu(Serial3, PololuID);

/*##########################################

  //HOLD/HOME : 907 314 485
  //MB: 111 246 366
  //IB : 2 264 345
  //PLACE ON STACK : 268 204 620
  //MOVE AWAY: 268 204 640

  ##########################################*/

int ConstPos[5][3] = {{907, 314, 485}, {111, 246, 366}, {2, 264, 345}, {268, 204, 620}, {268, 204, 640}};

/*##########################################

  Arm State Constants -

  ##########################################*/

#define Default 0
#define MidAir  1
#define InsideBlock 2
#define PlaceOnStack  3
#define MoveAway  4

/*##########################################

  Robot Base Class -

  ##########################################
*/

class RobotBase
{
    int PwmPins[4];
    int PwmVal[4];
    int DirPins[4];
    bool DirVal[4];
    void MoveMotor(int MotorNumber);
  public:
    void InitRobotBase(int PwmPins[4], int DirPins[4]);
    void Move(int PWM, float angle);
    void ModPWM(int MotorNumber, int correction);
    void Rotate(String Direction, int pwm);
};

void RobotBase :: InitRobotBase(int PwmPins[4], int DirPins[4])
{
  for (int i = 0; i < 4; i++)
  {
    this->PwmPins[i] = PwmPins[i];
    this->DirPins[i] = DirPins[i];
    this->PwmVal[i] = 0;
    this->DirVal[i] = LOW;
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(this->PwmPins[i], OUTPUT);
    pinMode(this->DirPins[i], OUTPUT);
  }
}

void RobotBase :: Move(int PWM, float angle)
{
  angle = ((angle * PI) / 180.0 );
  int PwmVector[4] = {(int)(PWM * sin(angle)), (int)(PWM * cos(angle)), (int)( PWM * sin(angle)), (int)( PWM * cos(angle))};
  unsigned int PwmMagVector[4];
  bool DirVector[4] = {LOW, LOW, LOW, LOW};
  for (int i = 0; i < 4; i++)
  {
    DirVector[i] = ((PwmVector[i] >= 0) ? HIGH : LOW);
    PwmMagVector[i] = abs(PwmVector[i]);
  }
  for (int i = 0; i < 4; i++)
  {
    this->PwmVal[i] = PwmMagVector[i];
    this->DirVal[i] = DirVector[i];
  }
  for (int i = 0; i < 4; i++)
  {
    MoveMotor(i);
  }
}

void RobotBase::MoveMotor(int MotorNumber)
{
  if (this->PwmVal[MotorNumber] > 255)
  {
    this->PwmVal[MotorNumber] = 255;
  }
  analogWrite(this->PwmPins[MotorNumber], this->PwmVal[MotorNumber]);
  digitalWrite(this->DirPins[MotorNumber], this->DirVal[MotorNumber]);
}

void RobotBase :: ModPWM(int MotorNumber, int correction)
{
  PwmVal[MotorNumber] += correction;
  PwmVal[MotorNumber] = abs(PwmVal[MotorNumber]);
  MoveMotor(MotorNumber);
}

void RobotBase::Rotate(String Direction, int pwm)
{
  int PwmVector[4] = {pwm , pwm , pwm , pwm};
  int PwmMagVector[4] = {pwm, pwm, pwm, pwm};
  bool DirVector[4] = {LOW, LOW, LOW, LOW};
  if (Direction == "AntiClockwise")
  {
    DirVector[0] = LOW;
    DirVector[1] = HIGH;
    DirVector[2] = HIGH;
    DirVector[3] = LOW;
  }
  else if (Direction == "Clockwise")
  {
    DirVector[0] = HIGH;
    DirVector[1] = LOW;
    DirVector[2] = LOW;
    DirVector[3] = HIGH;
  }
  for (int i = 0; i < 4; i++)
  {
    this->DirVal[i] = DirVector[i];
    this->PwmVal[i] = PwmMagVector[i];
  }
  for (int i = 0; i < 4; i++)
  {
    this->MoveMotor(i);
  }
}

/*####################################

  LSA Class

  ####################################*/

class LSA
{
    int CurVal;
    int PrevVal;
    int activePin;
    bool activeStatus;
    HardwareSerial *LSASerial;
    void clearLSA();
  public:
    void SetLSASerial(HardwareSerial*, int, bool);
    int RetCurVal();
    int RetPrevVal();
};

void LSA :: clearLSA()
{
  char junk;
  while (LSASerial->available())
  {
    junk = LSASerial->read();
  }
}

void LSA :: SetLSASerial(HardwareSerial *serial, int pin, bool Stat)
{
  LSASerial = serial;
  activeStatus = Stat;
  activePin = pin;
  pinMode(activePin, OUTPUT);
  digitalWrite(activePin, activeStatus);
  LSASerial->begin(38400);
  PrevVal = 0;
  CurVal = 0;
}

int LSA :: RetCurVal()
{
  digitalWrite(activePin, activeStatus);
  clearLSA();
  while (!(LSASerial->available()));
  while (LSASerial->available())
  {
    PrevVal = CurVal;
    CurVal = LSASerial->read();
  }
  clearLSA();
  return CurVal;
}

int LSA :: RetPrevVal()
{
  return PrevVal;
}

/*##########################################

  Variables -

  ##########################################*/

Servo SM_1, SM_2;
Servo ArmServo;
LSA L1, L2;
RobotBase BaseVariable;
int BotAngle;

/*#########################################################

  Sensor Functions -

  ###########################################################*/


void Blink(int n)
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(IndicatorLED, HIGH);
    delay(1000);
    digitalWrite(IndicatorLED, LOW);
    delay(1000);
  }
}

void InitSensors()
{
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(Limit_1, INPUT);
  pinMode(Limit_2, INPUT);
  pinMode(IndicatorLED, OUTPUT);
  SM_1.attach(SM_1_Pin);
  SM_2.attach(SM_2_Pin);
  SM_1.write(SM_1_Lower);
  SM_2.write(SM_2_Lower);
}

int Ultrasonic()
{
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  int distance = pulseIn(EchoPin, HIGH) * 0.0343 / 2;
  if (distance > 1000)
  {
    return Ultrasonic();
  }
  else
  {
    return distance;
  }
}

bool LimitSwitch(int Limit_Case)
{
  return (digitalRead(Limit_Case));
}

void SpecialMechanism()
{
  SM_1.write(SM_1_Upper);
  SM_2.write(SM_2_Upper);
  delay(1000);
  SM_1.write(SM_1_Lower);
  SM_2.write(SM_2_Lower);
}

/*#################################################

  Bot Motion Functions -

  #################################################*/

void BaseMove(String cmd, bool snapLine = false)
{
  if (cmd == "Kill" )
  {
    BaseVariable.Move(0, 0);
    BasePWM = 150;
    delay(500);
  }

  if (cmd == "Forward")
  {
    BaseVariable.Move(BasePWM, 0);
    if (snapLine == true)
    {
      delay(800);
      do
      {
        BaseVariable.Move(BasePWM, 0);
      } while (L2.RetCurVal() != 35);
      BaseMove("Kill");

      if (L2.RetCurVal() < 35)
      {
        while ((L2.RetCurVal() != 35))
        {
          BaseVariable.Move(40, 180);
        }
      }
      BaseMove("Kill");
    }
  }

  if (cmd == "Backward")
  {
    BaseVariable.Move(BasePWM, 180);
    if (snapLine == true)
    {
      delay(800);
      do
      {
        BaseVariable.Move(BasePWM, 180);
      } while (L2.RetCurVal() != 35);
      BaseMove("Kill");

      if (L2.RetCurVal() < 35)
      {
        while ((L2.RetCurVal() != 35))
        {
          BaseVariable.Move(80, 0);
        }
      }
      BaseMove("Kill");
    }
  }

  if (cmd == "Right")
  {
    BaseVariable.Move(BasePWM, 270);
    if (snapLine == true)
    {
      delay(800);
      do
      {
        BaseVariable.Move(BasePWM, 270);
      } while (!(L1.RetCurVal() >= 30 && L1.RetCurVal() <= 40));
      BaseMove("Kill");

      if (L1.RetCurVal() < 35)
      {
        while ((L1.RetCurVal() != 35))
        {
          BaseVariable.Move(80, 90);
          BaseVariable.ModPWM(0, -0.1 * 80);
        }
      }
      BaseMove("Kill");
    }
  }

  if (cmd == "Left")
  {
    BaseVariable.Move(BasePWM, 90);
    BaseVariable.ModPWM(0, -0.1 * BasePWM);
    if (snapLine == true)
    {
      delay(800);
      do
      {
        BaseVariable.Move(BasePWM, 90);
        BaseVariable.ModPWM(0, -0.1 * BasePWM);
      } while (L1.RetCurVal() != 35);
      BaseMove("Kill");

      if (L1.RetCurVal() < 35)
      {
        while ((L1.RetCurVal() != 35))
        {
          BaseVariable.Move(80, 270);
          BaseVariable.ModPWM(0, -0.1 * 80);
        }
      }
      BaseMove("Kill");
    }
  }
}

void JunctionCorrection()
{
  while (L1.RetCurVal() != 35)
  {
    if (L1.RetCurVal() > 35)
    {
      BasePWM = 50;
      BaseMove("Right");
    }
    if (L1.RetCurVal() < 35)
    {
      BasePWM = 50;
      BaseMove("Left");
    }
  }
  BaseMove("Kill");

  while (L2.RetCurVal() != 35)
  {
    if (L2.RetCurVal() < 35)
    {
      BasePWM = 50;
      BaseMove("Backward");
    }
    if (L2.RetCurVal() > 35)
    {
      BasePWM = 50;
      BaseMove("Forward");
    }
  }

  while (L1.RetCurVal() != 35)
  {
    if (L1.RetCurVal() > 35)
    {
      BasePWM = 50;
      BaseMove("Right");
    }
    if (L1.RetCurVal() < 35)
    {
      BasePWM = 50;
      BaseMove("Left");
    }
  }
  BaseMove("Kill");

  while (L2.RetCurVal() != 35)
  {
    if (L2.RetCurVal() < 35)
    {
      BasePWM = 50;
      BaseMove("Backward");
    }
    if (L2.RetCurVal() > 35)
    {
      BasePWM = 50;
      BaseMove("Forward");
    }
  }
  BasePWM = 150;
  BaseMove("Kill");
}

void BaseRotate(String cmd)
{
  BaseVariable.Rotate(cmd, 80);
  delay(1000);

  do
  {
    BaseVariable.Rotate(cmd, 80);
  } while ((L1.RetCurVal() != 35 && L2.RetCurVal() != 35));

  BaseMove("Kill");

  BaseVariable.Rotate(cmd, 80);
  if (cmd == "AntiClockwise")
  {
    delay(200);
  }
  else
  {
    delay(130);
  }
  JunctionCorrection();
}

/*#########################################

  Arm Functions -

  #########################################*/

void ArmInit()
{
  Dynamixel.begin(1000000, DynSelectPin);
  Serial3.begin(115200);  //Pololu Serial
  ArmServo.attach(ArmServoPin);
  pinMode(RX3Pin, INPUT_PULLUP);
  ArmServo.write(60);
}

void ActuateArmTo(int Command)
{
  delay(200);
  Dynamixel.moveSpeed(DynaxID1, ConstPos[Command][2], 60);
  delay(200);
  Dynamixel.moveSpeed(DynaxID2, ConstPos[Command][1], 60);
  BasePololu.setPosition(ConstPos[Command][0], 150);
}

void ArmGripper(bool Status)
{
  if (Status == true)
  {
    ArmServo.write(60);   //open
  }
  else
  {
    ArmServo.write(78);   //close
  }
}

/*#########################################

  State Machine -

  #########################################*/

void GoHome()
{
  BaseRotate("AntiClockwise");
  BaseRotate("AntiClockwise");
  BaseMove("Kill");

  while ((LimitSwitch(Limit_1) & LimitSwitch(Limit_2)) != LOW)
  {
    BasePWM = 100;
    BaseMove("Right");
  }
  BaseMove("Kill");

  while (Ultrasonic() > 5)
  {
    BasePWM = 150;
    BaseMove("Forward");
  }
  BaseMove("Kill");

  BaseMove("Forward");
  delay(3000);
  BaseMove("Kill");

  ActuateArmTo(Default);
  ActuateArmTo(Default);
  BaseMove("Kill");
  while (1)
  {

  }
}

void MountPiece(bool special = false, char dir = 'r')
{
  if (special)
  {
    BaseRotate("AntiClockwise");
    BaseMove("Kill");

    BaseMove("Kill");
    JunctionCorrection();
    do
    {
      BasePWM = 80;
      BaseMove("Right");
    } while ((LimitSwitch(Limit_1) & LimitSwitch(Limit_2)) != LOW);
    BaseMove("Kill");


    BasePWM = 60;
    BaseMove("Backward");
    delay(200);
    BaseMove("Kill");

    SpecialMechanism();
    delay(800);
    SpecialMechanism();

    BasePWM = 60;
    BaseMove("Forward");
    delay(500);
    BaseMove("Kill");


    BaseMove("Left", true);
    BaseMove("Kill");
    JunctionCorrection();
    delay(200);

    BaseRotate("AntiClockwise");
    BaseMove("Kill");
    JunctionCorrection();

    BaseVariable.Rotate("Clockwise", 50);
    delay(250);
    BaseMove("Kill");

    BasePWM = 150;
    BaseMove("Backward");
    delay(1500);
    BaseMove("Kill");

    BaseMove("Forward", true);
    BaseMove("Kill");

    BaseRotate("AntiClockwise");
    BaseMove("Kill");
    JunctionCorrection();

    BaseRotate("AntiClockwise");
    BaseMove("Kill");
    JunctionCorrection();

    return ;
  }

  while (Ultrasonic() > 20)
  {
    BasePWM = 80;
    BaseMove("Forward");
  }
  BaseMove("Kill");

  ActuateArmTo(PlaceOnStack);
  delay(1000);
  ArmGripper(true);
  delay(1000);
  ActuateArmTo(MoveAway);
  delay(1000);
  ArmGripper(true);
  delay(1000);
  BasePWM = 40;
  BaseMove("Backward");
  delay(1000);
  ActuateArmTo(Default);
  if (dir == 'l')
  {
    BasePWM = 80;
    BaseMove("Left");
    delay(200);
    BaseMove("Kill");
  }
  BaseMove("Backward", true);
}

void PickupPiece()
{
  ArmGripper(true);
  ActuateArmTo(Default);
  delay(500);
  ActuateArmTo(MidAir);
  delay(1000);
  for (int i = 0; i < 3; i++)
  {
    BasePWM = 60;
    BaseMove("Right");
    delay(200);
    BasePWM = 60;
    BaseMove("Left");
    delay(200);
  }
  BaseMove("Kill");
  ActuateArmTo(InsideBlock);
  delay(1000);
  ArmGripper(false);
  delay(1000);
  ArmGripper(false);
  delay(1000);
  ActuateArmTo(MidAir);
  delay(500);
  ActuateArmTo(InsideBlock);
  delay(500);
  ActuateArmTo(Default);
  delay(1000);
  ArmGripper(true);
  delay(800);
  ArmGripper(false);
  delay(800);
}

void Block1and2()
{
  BaseMove("Forward", true);
  BaseMove("Kill");

  BaseMove("Right", true);
  BaseMove("Kill");
  JunctionCorrection();

  BaseMove("Forward", true);
  BaseMove("Kill");
  if (L1.RetCurVal() == 255)
  {
    BasePWM = 80;
    while (L1.RetCurVal() != 35)
    {
      BaseMove("Left");
    }
  }
  BaseMove("Kill");
  JunctionCorrection();
  MountPiece(true);
}

void Block()
{
  if (millis() >= TimeConstraint)
  {
    GoHome();
  }
  do
  {
    BasePWM = 150;
    BaseMove("Right");
  } while (Ultrasonic() > 35);
  BaseMove("Kill");

  // Pickup Piece Correction -
  BasePWM = 80;
  BaseMove("Right");
  delay(250);
  BaseMove("Kill");

  while (Ultrasonic() != 14)
  {
    BasePWM = 80;
    if (Ultrasonic() > 14)
    {
      BaseMove("Forward");
    }
    if (Ultrasonic() < 14)
    {
      BaseMove("Backward");
    }
  }
  BaseMove("Kill");

  PickupPiece();

  delay(800);
  BaseMove("Left", true);
  BaseMove("Kill");
  JunctionCorrection();
}

void Chute()
{
  if (millis() >= TimeConstraint)
  {
    GoHome();
  }
  BaseRotate("Clockwise");
  JunctionCorrection();
  do
  {
    BaseVariable.Rotate("Clockwise", 80);
  } while ((L1.RetCurVal() != 35 && L2.RetCurVal() != 35));
  JunctionCorrection();

  while (Ultrasonic() != 16)
  {
    BasePWM = 80;
    if (Ultrasonic() > 16)
    {
      BaseMove("Forward");
    }
    if (Ultrasonic() < 16)
    {
      BaseMove("Backward");
    }
  }
  BaseMove("Kill");

  PickupPiece();

  delay(800);
  BaseMove("Backward", true);
  BaseMove("Kill");
  JunctionCorrection();
  BaseRotate("AntiClockwise");
  MountPiece();
}

void StateMachine()
{
	// Move to Junction 2 and Use Special Mechanism. Come Back to Junction 2.
  Block1and2();		

  // From Junction 2 Move to Right to Pickup Piece and Come Back to Junction 2.
  Block();

  // Mount the Piece on the Final Stack.
  MountPiece();

  // Come Back to Junction 1.
  BaseMove("Backward",true);
  BaseMove("Kill");
  
  // Go Back to the Starting Zone.
  GoHome();
}

/*############################################

  Main Loop and Setup -

  #############################################*/

void setup()
{
  Serial.begin(9600);
  // Initialize Motor Pins.
  BaseVariable.InitRobotBase(BasePwmPins, BaseDirPins);
  // Set LSA Serial and Select Line and Value.
  L1.SetLSASerial(&Serial2, 26, HIGH);
  L2.SetLSASerial(&Serial2, 26, LOW);
  // Initialize Additional Sensors and Mechanisms.
  InitSensors();
  // Initialize Arm Pololu, Dynamixel and Servo.
  ArmInit();
  // Reset Arm to Default Position.
  ActuateArmTo(Default);
  // Set PWM of the Base Motors to 0.
  BaseMove("Kill");
  // Indicator LED.
  Blink(2);
}

void loop()
{
  StateMachine();
}
