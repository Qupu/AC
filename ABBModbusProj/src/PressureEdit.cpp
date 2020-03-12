/*
 * PressureEdit.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include <PressureEdit.h>
#include "IntegerEdit.h"
#include "SystemUI.h"

PressureEdit::PressureEdit(LiquidCrystal *lcd_) : IntegerEdit(lcd_, "PRESSURE", step, lowLim, highLim) { }

PressureEdit::~PressureEdit() { }

void PressureEdit::display() {
	lcd->clear();

	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(SystemUI::lcdWidth-1, 0);
	lcd->print("A");

	lcd->setCursor(0,1);
	char s[10];
	snprintf(s, 10, "[%3d]", edit);
	lcd->print(s);

	snprintf(s, 10, "%3d Pa", (int)(currPressure+0.5));
	lcd->setCursor(SystemUI::lcdWidth-6, 1);
	lcd->print(s);
}


