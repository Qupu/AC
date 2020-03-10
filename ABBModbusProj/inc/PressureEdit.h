/*
 * PressureEdit.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef PRESSUREEDIT_H_
#define PRESSUREEDIT_H_

#include "DecimalEdit.h"
#include "LiquidCrystal.h"

class PressureEdit : public DecimalEdit {
public:
	PressureEdit(LiquidCrystal *lcd_);
	virtual ~PressureEdit();
	void display();
private:
	const double step    = 0.5;
	const double lowLim  = 0.0;
	const double highLim = 0.0;
};

#endif /* PRESSUREEDIT_H_ */
