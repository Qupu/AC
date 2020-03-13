/*
 * PressureEdit.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef PRESSUREEDIT_H_
#define PRESSUREEDIT_H_

#include "IntegerEdit.h"
#include "LiquidCrystal.h"

class PressureEdit : public IntegerEdit {
public:
	PressureEdit(LiquidCrystal *lcd_);
	virtual ~PressureEdit();
	void display();
	bool getFocus() {return true;}
	void setCurrPressure(double _currPressure) { currPressure = _currPressure; }

private:
	static const int step = 1;
	static const int lowLim  = 0;
	static const int highLim = 120;

	double currPressure;
};

#endif /* PRESSUREEDIT_H_ */
