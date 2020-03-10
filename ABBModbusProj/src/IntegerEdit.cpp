#include "IntegerEdit.h"
#include <cstdio>

IntegerEdit::IntegerEdit(LiquidCrystal *lcd_, std::string editTitle, int _step, int _lowerLim, int _upperLim): lcd(lcd_), title(editTitle) {
	value = 0;
	edit = 0;
	step = _step;
	lowerLim = _lowerLim;
	upperLim = _upperLim;
	focus = false;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	edit += step;
	if (edit < lowerLim || edit > upperLim)
		edit -= step;
}

void IntegerEdit::decrement() {
	edit -= step;
	if (edit < lowerLim || edit > upperLim)
		edit += step;
}

bool IntegerEdit::accept() {
	return save();
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
}

bool IntegerEdit::getFocus() {
	return true;
}

void IntegerEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%4d]     ", edit);
	}
	else {
		snprintf(s, 17, "      %4d      ", edit);
	}
	lcd->print(s);
}


bool IntegerEdit::save() {
	if (value != edit) {
		// set current value to be same as edit value
		value = edit;
		return true;
	}
	else {
		return false;
	}
	// todo: save current value for example to EEPROM for permanent storage
}


int IntegerEdit::getValue() {
	return value;
}
void IntegerEdit::setValue(int value) {
	if (value < lowerLim)
		value = lowerLim;
	else if (value > upperLim)
		value = upperLim;

	edit = value;
	save();
}

void IntegerEdit::setStep(int _step) {
	step = _step;
}
