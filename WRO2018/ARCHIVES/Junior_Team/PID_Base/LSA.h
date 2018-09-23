#include "Arduino.h"
class LSA
{
	int cur_val;
	int prev_val;
	HardwareSerial *LSASerial;
	void clearLSA();

	public:

	void SetLsaSerial(HardwareSerial*);
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

void LSA :: SetLsaSerial(HardwareSerial *serial)
{
	LSASerial = serial;
	LSASerial->begin(38400);
	prev_val = 0;
	cur_val = 0;
}

int LSA :: RetCurVal()
{
	clearLSA();
	while (!(LSASerial->available()));
	while (LSASerial->available())
	{
		prev_val = cur_val;
		cur_val = LSASerial->read();
	}
	clearLSA();
	return cur_val;
}

int LSA :: RetPrevVal()
{
	return prev_val;
}
