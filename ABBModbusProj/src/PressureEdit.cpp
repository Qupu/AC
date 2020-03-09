/*
 * PressureEdit.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include <PressureEdit.h>

PressureEdit::PressureEdit(LiquidCrystal *lcd_, std::string editTitle, int _lowerLim, int _upperLim) : DecimalEdit(lcd_, editTitle, _lowerLim, _upperLim) {
	// TODO Auto-generated constructor stub

}

PressureEdit::~PressureEdit() {
	// TODO Auto-generated destructor stub
}

