/*
 * PressureEdit.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include <PressureEdit.h>
#include "DecimalEdit.h"
#include "SystemUI.h"

PressureEdit::PressureEdit(LiquidCrystal *lcd_) : DecimalEdit(lcd_, "PRESSURE", step, lowLim, highLim) { }

PressureEdit::~PressureEdit() { }

void PressureEdit::display() {
	lcd->clear();

	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0, SystemUI::lcdWidth-1);
	lcd->print("A");

	lcd->setCursor(0,1);
	char s[17];
	snprintf(s, 17, "      %.1f      ", edit);

	lcd->print(s);
}


