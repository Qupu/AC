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
FrequencyEdit::FrequencyEdit(LiquidCrystal *lcd_) : IntegerEdit(lcd_, "FREQUENCY", step, lowLim, upLim) {
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
	lcd->setCursor(0, SystemUI::lcdWidth-1);
	lcd->print("M");
	lcd->setCursor(0,1);

	char s[10];
	snprintf(s, 10, "%.2f", edit);
	graph.draw(edit);
	lcd->print(s);
}
