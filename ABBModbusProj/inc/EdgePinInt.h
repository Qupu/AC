/*
 * EdgePinInt.h
 *
 *  Created on: 29 Feb 2020
 *      Author: Veli-Pekka Vaikkola
 */

#ifndef EDGEPININT_H_
#define EDGEPININT_H_

#include "DigitalIoPin.h"

class EdgePinInt : public DigitalIoPin {
public:
	EdgePinInt(int int_nr, int port, int pin, bool high = true);
	virtual ~EdgePinInt();
private:
	static int instances;
};

#endif /* EDGEPININT_H_ */
