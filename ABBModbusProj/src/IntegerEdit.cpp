/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>

IntegerEdit::IntegerEdit(LiquidCrystal *lcd_, std::string editTitle, int _lowerLim, int _upperLim): lcd(lcd_), title(editTitle) {
	value = 0;
	edit = 0;
	lowerLim = _lowerLim;
	upperLim = _upperLim;
	focus = false;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	if (edit < upperLim)
		edit++;
}

void IntegerEdit::decrement() {
	if (edit > lowerLim)
		edit--;
}

bool IntegerEdit::accept() {
	return save();
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool IntegerEdit::getFocus() {
	return this->focus;
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
