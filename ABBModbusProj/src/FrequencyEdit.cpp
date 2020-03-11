/*
 * FrequencyEdit.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include "FrequencyEdit.h"
#include "SystemUI.h"
#include "IntegerEdit.h"

// TODO: Adjust the step for the edit menu
FrequencyEdit::FrequencyEdit(LiquidCrystal *lcd_) : IntegerEdit(lcd_, "SPEED", step, lowLim, highLim) {
	// Create the bar graph for the FrequencyEdit display view:
	graph = new BarGraph(lcd_, barWidth);
}

FrequencyEdit::~FrequencyEdit() {
	delete graph;
}

void FrequencyEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(SystemUI::lcdWidth-1, 0);
	lcd->print("M");
	lcd->setCursor(0,1);

	char s[10];
	snprintf(s, 10, "%3d Pa", (int)(currPressure+0.5));
	graph->draw(edit);
	lcd->setCursor(SystemUI::lcdWidth-6, 1);
	lcd->print(s);
}
