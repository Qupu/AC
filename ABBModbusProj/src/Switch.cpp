/*
 * Switch.cpp
 *
 *  Created on: 6.3.2020
 *      Author: rqqht
 */

#include "Switch.h"

extern void Sleep(int ms);

Switch::Switch(int _port, int _pin, bool _input, bool _pullup, bool _invert) : DigitalIoPin(_port, _pin, _input, _pullup, _invert) {
	// TODO Auto-generated constructor stub

}

Switch::~Switch() {
	// TODO Auto-generated destructor stub
}

bool Switch::read() {
	int logicalOne = 0;
	int logicalZero = 0;

	while (logicalOne < 3 && logicalZero < 3) {
		if (DigitalIoPin::read()) {
			logicalOne++;
			logicalZero = 0;
		} else {
			logicalZero++;
			logicalOne = 0;
		}

		Sleep(10);
	}

	return (logicalOne > logicalZero);
}
