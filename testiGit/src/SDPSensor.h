/*
 * SDPSensor.h
 *
 *  Created on: 25 Feb 2020
 *      Author: Veli-Pekka Vaikkola
 */

#ifndef SDPSENSOR_H_
#define SDPSENSOR_H_

#include <cstdio>
#include "ITMWrapper.h"
#include "I2C.h"
#include "board.h"

class SDPSensor {
public:
	static const uint8_t cmd_measure = 0xF1;
	static const uint8_t cmd_reset = 0xFE;
	static const int16_t read_fail = 0x8000;

	SDPSensor(uint8_t addr, int p_ambient = 1013, ITMWrapper * itm = nullptr);
	virtual ~SDPSensor();
	double GetPressure();
	bool SoftReset();
	void SetAmbientPressure(int p_ambient);

private:
	/* Pressure = sensor output / scale factor
	 * SDP6xx-500Pa & SDP5xx	60
	 * SDP6x0-125Pa				240  <-
	 * SDP6x0-25Pa				1200
	 */
	const int scale_factor;
	const double cal_pressure;	// Calibration pressure in mbar (hPa)

	const uint8_t address;		// I2C device address
	double alt_corr_factor;		// Calibration pressure / (Ambient pressure * Scale factor);

	I2C * i2c;
	ITMWrapper * itm;

	static uint8_t Crc8(uint8_t *data, int size);
};


#endif /* SDPSENSOR_H_ */
