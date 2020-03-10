
#ifndef DECIMALEDIT_H_
#define DECIMALEDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>

class DecimalEdit : public PropertyEdit {
public:
	DecimalEdit(LiquidCrystal *lcd_, std::string editTitle, float _step, float _lowerLim, float _upperLim);
	virtual ~DecimalEdit();
	void increment();
	void decrement();
	bool accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	float getValue();
	void setValue(float value);
	void setStep(float _step);
protected:
	bool save();
	void displayEditValue();
	LiquidCrystal *lcd;
	std::string title;
	float value;
	float edit;
	float step;
	float lowerLim;
	float upperLim;
	bool focus;
	static constexpr float tolerance = 0.001;
};

#endif /* DECIMALEDIT_H_ */
