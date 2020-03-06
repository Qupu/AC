/*
 * SDPSensor.cpp
 *
 *  Created on: 25 Feb 2020
 *      Author: Veli-Pekka Vaikkola
 */

#include "SDPSensor.h"

SDPSensor::SDPSensor(uint8_t addr, double p_ambient) : address(addr) {
	I2C_config conf;
	conf.clock_divider = 72;	// 100 KHz
	i2c = new I2C(conf);

	alt_corr_factor = cal_pressure / (p_ambient * scale_factor);
};

SDPSensor::~SDPSensor() {
	delete i2c;
};

int SDPSensor::ReadPressure(double & pressure) {
	uint8_t txBuff = cmd_measure;
	uint8_t rxBuff[3] = {};
	int16_t measurement = 0;
	int ret;

	if (i2c->transaction(address, &txBuff, 1, rxBuff, 3)) {
		// Extract measurement from received data
		measurement = (rxBuff[0] << 8) | rxBuff[1];

		ret = (Crc8(rxBuff, 2) == rxBuff[2]) ? read_success : crc_mismatch;

		// Calculate actual pressure
		pressure = measurement * alt_corr_factor;
	} else {
		ret = i2c_fail;
	}

	return ret;
};

bool SDPSensor::SoftReset() {
	uint8_t txBuff = cmd_reset;

	return i2c->write(address, &txBuff, 1);
}

void SDPSensor::SetAmbientPressure(double p_ambient) {
	alt_corr_factor = cal_pressure / (p_ambient * scale_factor);
}

/* Generator polynomial: x^8 + x^5 + x^4  +  x^0
 * 						   8 7 6 5     4 3 2 1 0
 *					   --> 1 0 0 1     1 0 0 0 1 = 0x131
 */
uint8_t SDPSensor::Crc8(uint8_t *data, int size) {
	uint8_t crc = 0x00;

	while (size--) {
		crc ^= *data++;
		int i = 8;
		while (i--) {
			crc = (crc & 0x80) ? (crc << 1) ^ 0x131 : (crc << 1);
		}
	}

	return crc;
}


