
#ifndef DECIMALEDIT_H_
#define DECIMALEDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

class DecimalEdit : public PropertyEdit {
public:
	DecimalEdit(LiquidCrystal *lcd_, std::string editTitle, double _step, double _lowerLim, double _upperLim);
	virtual ~DecimalEdit();
	void increment();
	void decrement();
	bool accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	double getValue();
	void setValue(double value);
	void setStep(double _step);
protected:
	bool save();
	void displayEditValue();
	LiquidCrystal *lcd;
	std::string title;
	double value;
	double edit;
	double step;
	double lowerLim;
	double upperLim;
	bool focus;
	static constexpr double tolerance = 0.001;
};

#endif /* DECIMALEDIT_H_ */
