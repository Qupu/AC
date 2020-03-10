/*
 * SystemMenu.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include "SystemUI.h"
#include "DigitalIoPin.h"
#include "MenuItem.h"
#include <exception>

struct InvalidModeException : public std::exception {
   const char * what () const throw () {
      return "Invalid operation mode";
   }
};

// Private Methods:
void SystemUI::switchMode() {
	currMenu->reset();

	if (mode == OperationMode::AUTOMATIC){
			mode = OperationMode::MANUAL;
			currMenu = &manualModeMenu;
	}
	else {
		mode = OperationMode::AUTOMATIC;
		currMenu = &autoModeMenu;
	}

	currMenu->event(MenuItem::menuEvent::show);
}

void SystemUI::displayPowerOff() {
/*	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("POWER OFF...");*/
	printf("POWER OFF...");
}

// ---------------------
// The public interface:

SystemUI::SystemUI(bool _powerOn) {
	//Set up the LCD display:
	DigitalIoPin RS(0, 8, false, false, false);
	DigitalIoPin EN(1, 6, false, false, false);
	DigitalIoPin D0(1, 8, false, false, false);
	DigitalIoPin D1(0, 5, false, false, false);
	DigitalIoPin D2(0, 6, false, false, false);
	DigitalIoPin D3(0, 7, false, false, false);

	lcd = new LiquidCrystal(&RS, &EN, &D0, &D1, &D2, &D3);
	lcd->begin(lcdWidth, lcdHeight);
	lcd->clear();
	lcd->setCursor(0,0);

	// Set up the manual and automatic mode menus:
	pressureEdit = new PressureEdit(lcd);
	autoModeMenu.addItem(new MenuItem(pressureEdit));
	pressureEdit->setValue(50.0);

	frequencyEdit = new FrequencyEdit(lcd);
	manualModeMenu.addItem(new MenuItem(frequencyEdit));
	frequencyEdit->setValue(50);

	currMenu = &autoModeMenu;

	powerOn = _powerOn;
}

SystemUI::~SystemUI() {
	delete pressureEdit;
	delete frequencyEdit;
	delete lcd;
}

void SystemUI::event(systemUIEvent e) {
	switch (e) {
		case (systemUIEvent::MODE_SW_PRESSED):
			if (powerOn)
				switchMode();
			break;

		case (systemUIEvent::UP_SW_PRESSED):
			if (powerOn) {
				currMenu->event(MenuItem::menuEvent::up);
				currMenu->event(MenuItem::menuEvent::ok);
			}
			break;

		case (systemUIEvent::DOWN_SW_PRESSED):
			if (powerOn) {
				currMenu->event(MenuItem::menuEvent::down);
				currMenu->event(MenuItem::menuEvent::ok);
			}
			break;

		case (systemUIEvent::SELECT_SW_PRESSED):
			// No functionality implemented
			// Could be used if there were more menu items
			// for each mode's menu.
			break;

		case (systemUIEvent::POWER_SW_PRESSED):
			powerOn = !powerOn;
			break;

		case (systemUIEvent::SHOW):
			if (powerOn) {
				currMenu->event(MenuItem::menuEvent::show);
			}
			else {
				displayPowerOff();
			}
			break;
	}
}

OperationMode SystemUI::getOperationMode() {
	return mode;
}

double SystemUI::getPressure() {
	return pressureEdit->getValue();
}

int SystemUI::getFrequency() {
	frequencyEdit->getValue();
}
