#ifndef INTEGEREDIT_H_
#define INTEGEREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

class IntegerEdit: public PropertyEdit {
public:
	IntegerEdit(LiquidCrystal *lcd_, std::string editTitle, int _step, int _lowerLim, int _upperLim);
	virtual ~IntegerEdit();
	void increment();
	void decrement();
	bool accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	int getValue();
	void setValue(int value);
	void setStep(int _step);
private:
	bool save();
	void displayEditValue();
	LiquidCrystal *lcd;
	std::string title;
	int value;
	int edit;
	int step;
	int lowerLim;
	int upperLim;
	bool focus;
};

#endif /* INTEGEREDIT_H_ */
