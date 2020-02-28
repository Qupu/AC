/*
 * ACH550.h
 *
 *  Created on: 28.2.2020
 *      Author: Santeri Rauhala
 */

#ifndef ACH550_H_
#define ACH550_H_

class ACH550 {
public:
	ACH550();
	virtual ~ACH550();
	void accelerate();
	void decelerate();
	void stop();
	void begin();

	int getCurrSpeed();
	int getTargetSpeed();
	bool getState();

private:
	int CURRspeed;				//current running speed of the drive
	int TARGETspeed;			//desired speed
	bool state;					//true on , false off
	ModbusMaster mm;
	ModbusRegister mr;
};

#endif /* ACH550_H_ */
