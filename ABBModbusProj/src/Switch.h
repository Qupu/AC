/*
 * Switch.h
 *
 *  Created on: 6.3.2020
 *      Author: rqqht
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "DigitalIoPin.h"

class Switch : public DigitalIoPin {
public:
	Switch(int _port, int _pin, bool _input, bool _pullup, bool _invert);
	virtual ~Switch();
	bool read();
};

#endif /* SWITCH_H_ */
