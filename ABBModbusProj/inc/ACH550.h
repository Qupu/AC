/*
 * ACH550.h
 *
 *  Created on: 28.2.2020
 *      Author: Santeri Rauhala
 */
#include "ModbusMaster.h"
#include "ModbusRegister.h"

#ifndef ACH550_H_
#define ACH550_H_

class ACH550 {
public:
	ACH550(ModbusMaster mast);
	virtual ~ACH550();

	void stop();
	void begin();

	void updateSpeed();

	int getCurrSpeed();
	bool getState();
	void setTargetSpeed(int sp);
	void accelerate();
	void decelerate();

private:
	int CURRspeed;				//current running speed of the drive
	int TARGETspeed;			//desired speed
	bool state;					//true on , false off

	ModbusMaster mm;
	//ModbusRegister mr;
};

#endif /* ACH550_H_ */
