/*
 * SystemMenu.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include <SystemManager.h>
#include "DigitalIoPin.h"
#include "MenuItem.h"

// Private Methods:

// SwitchMode switches the operation mode between
// MANUAL and AUTOMATIC and makes all the required
// adjustments to the menus
void SystemManager::switchMode() {
	// Although the SimpleMenu reset method is not currently used for anything,
	// it could be used to perform any clean-up initializations when switching
	// from one mode's menu to another mode's menu in a complete commercial system
	// with more complex menu hierarchy.
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

void SystemManager::displayPowerOff() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("POWER OFF...");
}

void SystemManager::displayLatencyError() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print("TARGET PRESSURE");
	lcd->setCursor(0,1);
	lcd->print("NOT REACHABLE");
}

// ---------------------
// The public interface:

SystemManager::SystemManager(bool _powerOn) :
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

SystemManager::~SystemManager() {
	delete frequencyEdit;
	delete pressureEdit;
	delete lcd;
}

void SystemManager::event(SystemEvent e) {
	// The following holds for all button press events:
	// if the system is in an error state and an event
	// is fired, the error status is checked/cancelled
	// (by using the SystemEvent::ERROR_ACK-event)

	switch (e) {
		case (SystemEvent::MODE_SW_PRESSED):
			// The MODE_SW_PRESSED event is used to change the System's operation mode

			if (errorStatus == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
				event(SystemEvent::ERROR_ACK);
			}
			else if (powerOn && errorStatus == ErrorStatus::NO_ERROR) {
				switchMode();
			}
			break;

		case (SystemEvent::UP_SW_PRESSED):
			// The UP_SW_PRESSED increases the value for the current mode's active menu item.
			// Since, each mode only has one menu item at the moment, it increases either
			// the target pressure (in AUTOMATIC mode) or the target speed/frequency (in MANUAL mode)

			if (errorStatus == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
						event(SystemEvent::ERROR_ACK);
					}
			else if (powerOn && errorStatus == ErrorStatus::NO_ERROR) {
				// Using the original SimpleMenu, every edit-event requires
				// also an ok event:
				currMenu->event(MenuItem::menuEvent::up);
				currMenu->event(MenuItem::menuEvent::ok);
			}
			break;

		case (SystemEvent::DOWN_SW_PRESSED):
			// The UP_SW_PRESSED decreases the value for the current mode's active menu item.
			// Since, each mode only has one menu item at the moment, it decreases either
			// the target pressure (in AUTOMATIC mode) or the target speed/frequency (in MANUAL mode)

			if (errorStatus == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
						event(SystemEvent::ERROR_ACK);
					}
			else if (powerOn && errorStatus == ErrorStatus::NO_ERROR) {
				currMenu->event(MenuItem::menuEvent::down);
				currMenu->event(MenuItem::menuEvent::ok);
			}
			break;

		case (SystemEvent::SELECT_SW_PRESSED):
			// No functionality implemented
			//
			// Could be used if there were more menu items for each mode's menu.
			//
			// A few possibilities:
			//   * "Show status" menu item
			//   * "Modify lower limit/upper limit" menu items
			//   * "Timed frequency boost" menu item for manual mode
			break;

		case (SystemEvent::POWER_SW_PRESSED):
			// the POWER_SW_PRESSED event is used to switch the power on/off.

			if (!powerOn) {
				// Turning power off negates all errors,
				// Thus no error handling needed when turning power on...
				powerOn = true;
				currMenu->event(MenuItem::menuEvent::show);
			}
			else {
				if (errorStatus == ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR) {
					event(SystemEvent::ERROR_ACK);
				}
				else {
					powerOn = false;
					errorStatus = ErrorStatus::NO_ERROR;
					displayPowerOff();
				}
			}
			break;

		case (SystemEvent::SHOW):
			// The SHOW event is used to show the current menu item if the
			// system is neither in error mode nor turned off.

			if (errorStatus == ErrorStatus::NO_ERROR){
				if (powerOn) {
					currMenu->event(MenuItem::menuEvent::show);
				}
				else {
					displayPowerOff();
				}
			}
			break;

		case (SystemEvent::TARGET_PRESSURE_LATENCY_ERROR):
			// The TARGET_PRESSURE_LATENCY_ERROR event is used to signal that the system
			// wasn't able to reach the target pressure in a reasonable time interval.

			if (powerOn) {
				errorStatus = ErrorStatus::TARGET_PRESSURE_UNREACHABLE_ERROR;
				displayLatencyError();
			}
			break;

		case (SystemEvent::ERROR_ACK):
			// The ERROR_ACK event is used to cancel/acknowledge a system error
			// So far there exists only one possible error type:
			// TARGET_PRESSURE_UNREACHABLE_ERROR...

			errorStatus = ErrorStatus::NO_ERROR;
			if (powerOn) {
				currMenu->event(MenuItem::menuEvent::show);
			}
			break;
	}
}

OperationMode SystemManager::getOperationMode() {
	return mode;
}

void SystemManager::setOperationMode(OperationMode _mode) {
	mode = _mode;
	if (mode == OperationMode::AUTOMATIC)
		currMenu = &autoModeMenu;
	else
		currMenu = &manualModeMenu;
}

double SystemManager::getTargetPressure() {
	return pressureEdit->getValue();
}

int SystemManager::getTargetFrequency() {
	return frequencyEdit->getValue();
}

void SystemManager::updateCurrPressure(double _currPressure) {
	currPressure = _currPressure;
	pressureEdit->setCurrPressure(_currPressure);
	frequencyEdit->setCurrPressure(_currPressure);

	event(SystemEvent::SHOW);
}
