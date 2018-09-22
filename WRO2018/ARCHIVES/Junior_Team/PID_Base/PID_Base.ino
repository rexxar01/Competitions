#include"Meccanum.h"
#include"LSA.h"
#include"PID.h"

// Base Class for Motion of the Base using Meccanum, LSAs and PID Classes.
class Motion
{
	Meccanum D;
	LSA L1, L2;
	PID P1, P2;

public:
	// Constructor for Assigning the Motor Driver Pins and PID Constants.
	Motion(int Dir_pins[], int Pwm_Pins[], double K1[], double K2[]);
	// Initialize LSAs and Mecanum Drive.
	void InitLsaBase(HardwareSerial *S1, HardwareSerial *S2);
	// Implements Line Following in the Given Direction.
	void FollowLine(String cmd, int pwm);
	// Applying PID Correction to the Base.
	void ApplyCorrection(String dir);
};

Motion :: Motion(int Dir_pins[], int Pwm_Pins[], double K1[], double K2[])
{
	// Assigning Pins in the Mecannum Class.
	D.AssignMeccanum(Dir_pins, Pwm_Pins);
	// Assign PID Constants.
	P1.AssignPID(K1[0], K1[1], K1[2]);
	P2.AssignPID(K2[0], K2[1], K2[2]);
}

void Motion :: InitLsaBase(HardwareSerial *S1, HardwareSerial *S2)
{
	// Setting LSA Serials.
	L1.SetLsaSerial(S1);
	L2.SetLsaSerial(S2);
	// Setting Up Mecanum Drive.
	D.Init();
}

void Motion :: FollowLine(String cmd, int pwm)
{
	// Attach Positive PWM to all the Motors.
	if (cmd == "Forward")
	{
		D.AttachPwm(0, pwm);
		D.AttachPwm(1, pwm);
		D.AttachPwm(2, pwm);
		D.AttachPwm(3, pwm);
		ApplyCorrection("Forward");
	}
	// Attach Negative PWM to all the Motors.
	else if (cmd == "Backward")
	{
		D.AttachPwm(0, -pwm);
		D.AttachPwm(1, -pwm);
		D.AttachPwm(2, -pwm);
		D.AttachPwm(3, -pwm);
		ApplyCorrection("Backward");
	}
	else if (cmd == "Right")
	{
		D.AttachPwm(0, -0.8 * pwm);
		D.AttachPwm(1, 0.55 * pwm);
		D.AttachPwm(2, -0.6 * pwm);
		D.AttachPwm(3, 0.8 * pwm);
		ApplyCorrection("Right");
	}
	else if (cmd == "Left")
	{
		D.AttachPwm(0, 0.85 * pwm);
		D.AttachPwm(1, -pwm);
		D.AttachPwm(2, pwm);
		D.AttachPwm(3, -0.85 * pwm);
		ApplyCorrection("Left");
	}
	else if (cmd == "RotateClockwise")
	{
		D.AttachPwm(0, 1.2 * pwm);
		D.AttachPwm(1, -0.8 * pwm);
		D.AttachPwm(2, -1.2 * pwm);
		D.AttachPwm(3, 0.8 * pwm);
	}
	else if (cmd == "RotateAntiClockwise")
	{
		D.AttachPwm(0, -pwm);
		D.AttachPwm(1, pwm);
		D.AttachPwm(2, pwm);
		D.AttachPwm(3, -pwm);
	}
	else
	{
		D.Kill();
	}
	// After Attaching and Modifying the PWM based on the PID Correction.
	// Move the Base.
	D.MoveMotor();
}

void Motion :: ApplyCorrection(String dir)
{
	int Val = 0;
	double Correction;
	if (dir == "Forward")
	{
		Val = L1.RetCurVal();
		if (Val == 255)
		{
			D.Kill();
		}
		// Get Correction Based on the Recieved LSA 1 value and Target value 35.
		Correction = P1.RetCorrection(Val, 35);
		D.ModPwm(0, -Correction);
		D.ModPwm(1, +Correction);
		D.ModPwm(2, -Correction);
		D.ModPwm(3, +Correction);
	}
	else if (dir == "Backward")
	{
		Val = L1.RetCurVal();
		if (Val == 255)
		{
			D.Kill();
		}
		// Get Correction Based on the Recieved LSA 1 value and Target value 35.
		Correction = P1.RetCorrection(Val, 35);
		D.ModPwm(0, -Correction);
		D.ModPwm(1, +Correction);
		D.ModPwm(2, -Correction);
		D.ModPwm(3, +Correction);
	}
	else if (dir == "Left")
	{
		Val = L2.RetCurVal();
		// Get Correction Based on the Recieved LSA 2 value and Target value 35.
		Correction = P2.RetCorrection(Val, 35);
		if (Val == 255)
		{
			D.Kill();
			int OldVal = L2.RetPrevVal();
			Val = L2.RetCurVal();
			while (!(Val >= 20 && Val <= 50))
			{
				if (OldVal > 35)
				{
					D.AttachPwm(0, 50);
					D.AttachPwm(1, 50);
					D.AttachPwm(2, 50);
					D.AttachPwm(3, 50);
				}
				else
				{
					D.AttachPwm(0, -50);
					D.AttachPwm(1, -50);
					D.AttachPwm(2, -50);
					D.AttachPwm(3, -50);
				}
				D.MoveMotor();
				Val = L2.RetCurVal();
			}
			return ;
		}
		D.ModPwm(0, -Correction);
		D.ModPwm(1, +Correction);
		D.ModPwm(2, -Correction);
		D.ModPwm(3, +Correction);
	}
	else if (dir == "Right")
	{
		Val = L2.RetCurVal();
		// Get Correction Based on the Recieved LSA 2 value and Target value 35.
		Correction = P2.RetCorrection(Val, 35);
		D.ModPwm(0, +Correction);
		D.ModPwm(1, -Correction);
		D.ModPwm(2, +Correction);
		D.ModPwm(3, -Correction);
	}
	D.MoveMotor();
}

int dir_pins[] = {43, 45, 49, 47}, pwm_pins[] = {5, 6, 7, 8};
double k1[] = {6, 0, 0}, k2[] = {1, 0, 0};

Motion M = Motion(dir_pins, pwm_pins, k1, k2);

void setup()
{
	M.InitLsaBase(&Serial1, &Serial2);
}

void loop()
{
  //M.FollowLine("Forward", 200);
  //M.FollowLine("Backward", 200);
  //M.FollowLine("Left", 200);
  //M.FollowLine("Right", 220);
  //M.FollowLine("RotateClockwise", 200);
  //M.FollowLine("RotateAntiClockwise", 200);

}
