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
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("POWER OFF...");
}

void SystemUI::displayLatencyError() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("TARGET PRESSURE");
	lcd->setCursor(0,2);
	lcd->print("NOT REACHABLE");

	lcd->setCursor(lcdWidth-1, 0);
	lcd->print("E");
	lcd->setCursor(0,1);
}

// ---------------------
// The public interface:

SystemUI::SystemUI(bool _powerOn) :
		RS(0, 8, false, false, false),
		EN(1, 6, false, false, false),
		D0(1, 8, false, false, false),
		D1(0, 5, false, false, false),
		D2(0, 6, false, false, false),
		D3(0, 7, false, false, false)
{
	//Set up the LCD display:
	lcd = new LiquidCrystal(&RS, &EN, &D0, &D1, &D2, &D3);
	lcd->begin(lcdWidth, lcdHeight);
	lcd->setCursor(0,0);

	// Set up the manual and automatic mode menus:
	pressureEdit = new PressureEdit(lcd);
	autoModeMenu.addItem(new MenuItem(pressureEdit));
	pressureEdit->setValue(0);

	frequencyEdit = new FrequencyEdit(lcd);
	manualModeMenu.addItem(new MenuItem(frequencyEdit));
	frequencyEdit->setValue(0);

	// General setups:
	powerOn = _powerOn;
	mode = OperationMode::MANUAL;
	currMenu = &manualModeMenu;
}

SystemUI::~SystemUI() {
	delete frequencyEdit;
	delete pressureEdit;
	delete lcd;
}

void SystemUI::event(systemUIEvent e) {

	switch (e) {
		case (systemUIEvent::MODE_SW_PRESSED):

			if (errorMode == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
				event(systemUIEvent::ERROR_ACK)
			}
			else if (powerOn && errorStatus == ErrorStatus::NO_ERROR) {
				switchMode();
			}
			break;

		case (systemUIEvent::UP_SW_PRESSED):

			if (errorMode == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
						event(systemUIEvent::ERROR_ACK)
					}
			else if (powerOn && errorStatus == ErrorStatus::NO_ERROR) {
				currMenu->event(MenuItem::menuEvent::up);
				currMenu->event(MenuItem::menuEvent::ok);
			}
			break;

		case (systemUIEvent::DOWN_SW_PRESSED):

			if (errorMode == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
						event(systemUIEvent::ERROR_ACK)
					}
			else if (powerOn && errorStatus == ErrorStatus::NO_ERROR) {
				currMenu->event(MenuItem::menuEvent::down);
				currMenu->event(MenuItem::menuEvent::ok);
			}
			break;

		case (systemUIEvent::SELECT_SW_PRESSED):
			// No functionality implemented
			//
			// Could be used if there were more menu items for each mode's menu.
			//
			// A few possibilities:
			//   * "Show status" menu item
			//   * "Modify lower limit/upper limit" menu items
			//   * "Timed frequency boost" menu item for manual mode
			break;

		case (systemUIEvent::POWER_SW_PRESSED):
			if (!powerOn) {
				// Turning power off negates all errors,
				// Thus no error handling needed when turning power on...
				powerOn = true;
				currMenu->event(MenuItem::menuEvent::show);
			}
			else {
				if (errorMode == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
					event(systemUIEvent::ERROR_ACK)
				}
				else {
					powerOn = false;
					errorStatus = ErrorStatus::NO_ERROR;
					displayPowerOff();
				}
			}
			break;

		case (systemUIEvent::SHOW):

			if (errorStatus == ErrorStatus::NO_ERROR){
				if (powerOn) {
					currMenu->event(MenuItem::menuEvent::show);
				}
				else {
					displayPowerOff();
				}
			}
			break;

		case (systemUIEvent::TARGET_PRESSURE_LATENCY_ERROR):

			if (powerOn) {
				errorStatus = ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR;
				displayLatencyError();
			}
			break;

		case (systemUIEvent::ERROR_ACK):

			errorStatus = ErrorStatus::NO_ERROR;
			if (powerOn) {
				currMenu->event(MenuItem::menuEvent::show);
			}
			break;
	}
}

OperationMode SystemUI::getOperationMode() {
	return mode;
}

void SystemUI::setOperationMode(OperationMode _mode) {
	mode = _mode;
	if (mode == OperationMode::AUTOMATIC)
		currMenu = &autoModeMenu;
	else
		currMenu = &manualModeMenu;
}

double SystemUI::getTargetPressure() {
	return pressureEdit->getValue();
}

int SystemUI::getTargetFrequency() {
	return frequencyEdit->getValue();
}

void SystemUI::updateCurrPressure(double _currPressure) {
	currPressure = _currPressure;
	pressureEdit->setCurrPressure(_currPressure);
	frequencyEdit->setCurrPressure(_currPressure);

	event(systemUIEvent::SHOW);
}
