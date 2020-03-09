/*
 * PressureEdit.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef PRESSUREEDIT_H_
#define PRESSUREEDIT_H_

#include "PropertyEdit.h"

class PressureEdit : public DecimalEdit {
public:
	PressureEdit(LiquidCrystal *lcd_, std::string editTitle, int _lowerLim, int _upperLim);
	virtual ~PressureEdit();
	void display();
};

#endif /* PRESSUREEDIT_H_ */
