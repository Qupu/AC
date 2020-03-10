/*
 * FrequencyEdit.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include <FrequencyEdit.h>
#include "SystemUI.h"
#include "IntegerEdit.h"
#include "SystemUI.h"

// TODO: Adjust the step for the edit menu
FrequencyEdit::FrequencyEdit(LiquidCrystal *lcd_) : IntegerEdit(lcd_, "FREQUENCY", 1, 0, 100) {
	// Create the bar graph for the FrequencyEdit display view:
	graph = new BarGraph(lcd_, barWidth);

}

FrequencyEdit::~FrequencyEdit() {
	// TODO Auto-generated destructor stub
}

void FrequencyEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0, SystemUI::lcdWidth-1);
	lcd->print("M");
	lcd->setCursor(0,1);

	// TODO: replace the code below with the BarGraph-drawing routine:
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%.1f]     ", edit);
	}
	else {
		snprintf(s, 17, "      %.1f      ", edit);
	}
	lcd->print(s);
}
