class PID
{
	// PID Constants.
	double kP, kD, kI;
	// Current and Previous Errors.
	double error, prev_error;
	// Integral and Derivative Factor for PID.
	double integral,derivative;
	// Resultant Correction.
	double corr;

	// Calculate Delta Difference between Target Value and Current Value.
	double CalcError(int Value,int Target);
	// Calculate the Integral Factor by Summing up all the Errors.
	double CalcIntegral();
	// Calculate the Derivative Factor by Finding the Difference between Current and Previous Errors.
	double CalcDerivative();

	public:

	// Purpose: Same as Constructor.
	void AssignPID(double kP,double kD,double kI);
	// Returns the Net Correction.
	double RetCorrection(int Value,int Target);
};

double PID :: CalcError(int Value,int Target)
{
	prev_error = error;
	error = Value - Target;
	return error;
}

double PID :: CalcIntegral()
{
	integral += error;
	return integral;
}

double PID :: CalcDerivative()
{
	derivative = error - prev_error;
	return derivative;
}

void PID :: AssignPID(double kP, double kD, double kI)
{
	this->kP = kP;
	this->kD = kD;
	this->kI = kI;
	error = 0;
	prev_error = 0;
	integral= 0;
	derivative = 0;
}

double PID :: RetCorrection(int Value,int Target)
{
	corr = kP * Calcerror(Value,Target) + kI * CalcIntegral() + kD * CalcDerivative();
	return corr;
}
