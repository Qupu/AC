/*
 * SDPSensor.cpp
 *
 *  Created on: 25 Feb 2020
 *      Author: Veli-Pekka Vaikkola
 */

#include "SDPSensor.h"

SDPSensor::SDPSensor(uint8_t addr, int p_ambient, ITMWrapper * itm) : scale_factor(240), cal_pressure(966), address(addr), itm(itm) {
	I2C_config conf{};
	i2c = new I2C(conf);

	// DPeff = DPsensor * (Pcal / Pamb)
	alt_corr_factor = cal_pressure / (p_ambient * scale_factor);
};

SDPSensor::~SDPSensor() {
	delete i2c;
};

double SDPSensor::GetPressure() {
	uint8_t txBuff = cmd_measure;
	uint8_t rxBuff[3] = {};
	int16_t measurement = 0;
	double pressure;

	if (i2c->transaction(address, &txBuff, 1, rxBuff, 3)) {
		// Extract measurement from received data
		measurement = (rxBuff[0] << 8) | rxBuff[1];

		// Check CRC if we can print to ITM console
		if (itm) {
			char str[40];
			snprintf(str, 40, "\nRaw data: 0x%X", measurement);
			itm->print(str);

			if (Crc8(rxBuff, 2) != rxBuff[2]) itm->print("\nCRC mismatch!");
			else itm->print("\nCRC OK");
		}

		// Calculate actual pressure
		pressure = measurement * alt_corr_factor;

		return pressure;
	}

	if (itm) itm->print("\nFailed to read pressure");
	return read_fail;
};

bool SDPSensor::SoftReset() {
	uint8_t txBuff = cmd_reset;

	if (itm) itm->print("\nSensor reset");
	return i2c->write(address, &txBuff, 1);
}

void SDPSensor::SetAmbientPressure(int p_ambient) {
	alt_corr_factor = cal_pressure / p_ambient / scale_factor;
	if (itm) itm->print("\nSensor reset");
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


