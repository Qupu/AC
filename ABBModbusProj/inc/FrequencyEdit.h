/*
 * FrequencyEdit.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef FREQUENCYEDIT_H_
#define FREQUENCYEDIT_H_

#include "IntegerEdit.h"
#include "LiquidCrystal.h"
#include "BarGraph.h"

class FrequencyEdit : public IntegerEdit {
public:
	static const int maxFrequency = 20000;
	static const int frequencyScale = 100;

	FrequencyEdit(LiquidCrystal *lcd_);
	virtual ~FrequencyEdit();
	void display();
	bool getFocus() { return true; }
	int getValue() { return (maxFrequency*value)/highLim; }
	void setValue(int _value) { IntegerEdit::setValue(_value/frequencyScale); }
	void setCurrPressure(double _currPressure) { currPressure = _currPressure; }

private:
	const int barWidth = 50;
	BarGraph *graph;

	double currPressure;

	static constexpr float step = 1;
	static constexpr float lowLim  = 0;
	static constexpr float highLim = 50;


};

#endif /* FREQUENCYEDIT_H_ */
