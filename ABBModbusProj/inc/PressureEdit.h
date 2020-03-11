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
	bool getFocus() {return true;}
	void setCurrPressure(double _currPressure) { currPressure = _currPressure; }

private:
	static constexpr float step    = 1.0;
	static constexpr float lowLim  = 0.0;
	static constexpr float highLim = 125.0;

	double currPressure;
};

#endif /* PRESSUREEDIT_H_ */
