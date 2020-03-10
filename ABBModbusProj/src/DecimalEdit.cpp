#include "DecimalEdit.h"
#include <cstdio>

DecimalEdit::DecimalEdit(LiquidCrystal *lcd_, std::string editTitle, float _step, float _lowerLim, float _upperLim): lcd(lcd_), title(editTitle) {
	value = 0;
	edit = 0;
	step = _step;
	lowerLim = _lowerLim;
	upperLim = _upperLim;
	focus = true;
}

DecimalEdit::~DecimalEdit() {
}

void DecimalEdit::increment() {
	edit += step;
	if (edit < lowerLim-tolerance || edit > upperLim+tolerance)
		edit -= step;
}

void DecimalEdit::decrement() {
	edit -= step;
	if (edit < lowerLim-tolerance || edit > upperLim+tolerance)
		edit += step;
}

bool DecimalEdit::accept() {
	return save();
}

void DecimalEdit::cancel() {
	edit = value;
}


void DecimalEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool DecimalEdit::getFocus() {
	return focus;
}

void DecimalEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%.1f]     ", edit);
	}
	else {
		snprintf(s, 17, "      %.1f      ", edit);
	}
	lcd->print(s);
}


bool DecimalEdit::save() {
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


float DecimalEdit::getValue() {
	return value;
}
void DecimalEdit::setValue(float value) {
	if (value < lowerLim)
		value = lowerLim;
	else if (value > upperLim)
		value = upperLim;

	edit = value;
	save();
}

void DecimalEdit::setStep(float _step) {
	step = _step;
}
