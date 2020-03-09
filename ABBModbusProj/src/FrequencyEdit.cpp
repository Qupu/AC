/*
 * FrequencyEdit.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include <FrequencyEdit.h>

FrequencyEdit::FrequencyEdit(LiquidCrystal *lcd_, std::string editTitle, int _lowerLim, int _upperLim) : IntegerEdit(lcd_, editTitle, _lowerLim, _upperLim) {
	// Create the bar graph for the FrequencyEdit display view:
	graph = new BarGraph(lcd_, barWidth);

}

FrequencyEdit::~FrequencyEdit() {
	// TODO Auto-generated destructor stub
}

