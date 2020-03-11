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
	FrequencyEdit(LiquidCrystal *lcd_);
	virtual ~FrequencyEdit();
	void display();
	bool getFocus() { return true; }

private:
	const int barWidth = 20;
	BarGraph *graph;

	static constexpr float step = 800;
		static constexpr float lowLim  = 0;
		static constexpr float highLim = 20000;
};

#endif /* FREQUENCYEDIT_H_ */
