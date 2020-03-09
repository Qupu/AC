/*
 * FrequencyEdit.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef FREQUENCYEDIT_H_
#define FREQUENCYEDIT_H_

#include "PropertyEdit.h"

class FrequencyEdit : public IntegerEdit {
public:
	FrequencyEdit(LiquidCrystal *lcd_, std::string editTitle, int _lowerLim, int _upperLim);
	virtual ~FrequencyEdit();
	void display();

private:
	const int barWidth = 20;
	BarGraph *graph;
};

#endif /* FREQUENCYEDIT_H_ */
