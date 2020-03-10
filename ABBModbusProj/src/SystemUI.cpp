/*
 * SystemMenu.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include "SystemUI.h"
#include "DigitalIoPin.h"
#include "MenuItem.h"

// Private Methods:
void SystemUI::switchMode() {
	currMenu->reset();

	switch (mode) {
		case (OperationMode::AUTOMATIC):
			mode = OperationMode::MANUAL;
			currMenu = &manualModeMenu;
			break;
		case (OperationMode::MANUAL):
			mode = OperationMode::AUTOMATIC;
			currMenu = &autoModeMenu;
			break;
	}

	currMenu->event(MenuItem::menuEvent::show);
}

// ---------------------
// The public interface:

SystemUI::SystemUI() {
	//Set up the LCD display:
	DigitalIoPin RS(0, 8, false, false, false);
	DigitalIoPin EN(1, 6, false, false, false);
	DigitalIoPin D0(1, 8, false, false, false);
	DigitalIoPin D1(0, 5, false, false, false);
	DigitalIoPin D2(0, 6, false, false, false);
	DigitalIoPin D3(0, 7, false, false, false);

	lcd = new LiquidCrystal(&RS, &EN, &D0, &D1, &D2, &D3);
	lcd->begin(lcdWidth, lcdHeight);
	lcd->setCursor(0,0);

	// Set up the manual and automatic mode menus:
	pressureEdit = new PressureEdit(lcd);
	autoModeMenu.addItem(new MenuItem(pressureEdit));

	frequencyEdit = new FrequencyEdit(lcd);
	manualModeMenu.addItem(new MenuItem(frequencyEdit));

}

SystemUI::~SystemUI() {
	delete pressureEdit;
	delete frequencyEdit;
	delete lcd;
}

void SystemUI::event(systemUIEvent e) {
	switch (e) {
		case (systemUIEvent::MODE_SW_PRESSED):
			switchMode();
			break;

		case (systemUIEvent::UP_SW_PRESSED):
			// Not implemented yet
			break;

		case (systemUIEvent::DOWN_SW_PRESSED):
			// Not implemented yet
			break;

		case (systemUIEvent::SELECT_SW_PRESSED):
			// No functionality implemented
			// Could be used if there were more menu items
			// for each mode's menu.
			break;

		case (systemUIEvent::POWER_SW_PRESSED):
			// Not implemented yet
			break;

		case (systemUIEvent::SHOW):
			currMenu->event(MenuItem::menuEvent::show);
			break;
	}
}

OperationMode SystemUI::getOperationMode() {
	return mode;
}
