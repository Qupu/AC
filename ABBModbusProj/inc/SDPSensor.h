/*
 * SDPSensor.h
 *
 *  Created on: 25 Feb 2020
 *      Author: Veli-Pekka Vaikkola
 */

#ifndef SDPSENSOR_H_
#define SDPSENSOR_H_

#include "board.h"
#include "I2C.h"

class SDPSensor {
public:
	enum { read_success, i2c_fail, crc_mismatch };
	static const uint8_t cmd_measure = 0xF1;
	static const uint8_t cmd_reset = 0xFE;

	/* p_ambient: Actual ambient absolute pressure, defaults to pressure at sea level */
	SDPSensor(uint8_t addr, double p_ambient = 1013);
	virtual ~SDPSensor();

	/* Calculates effective differential pressure from the sensor reading and writes it to reference argument
	 * if I2C transaction is successful.
	 * Returns read_success if successful, i2c_fail if I2C transaction fails, crc_mismatch if CRC doesn't match
	 */
	int ReadPressure(double & pressure);

	/* Returns true on successful I2C transaction, false otherwise */
	bool SoftReset();

	/* Calculates new altitude corrected scale factor for given ambient pressure */
	void SetAmbientPressure(double p_ambient);

private:
	/* Pressure = sensor output / scale factor
	 * SDP6xx-500Pa & SDP5xx	60
	 * SDP6x0-125Pa				240  <--
	 * SDP6x0-25Pa				1200
	 */
	static const int scale_factor = 240;

	/* Absolute pressure at calibration (966 mbar)  */
	static const int cal_pressure = 966;

	/* Altitude corrected scale factor. Calibration pressure / (Ambient pressure * Scale factor) */
	double alt_corr_factor;

	/* I2C device address */
	const uint8_t address;

	I2C * i2c;

	/* Calculates 8-bit CRC for the sensor value */
	static uint8_t Crc8(uint8_t *data, int size);
};


#endif /* SDPSENSOR_H_ */
