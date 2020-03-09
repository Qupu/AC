/*
 * SystemMenu.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include "SystemUI.h"

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
	// TODO Auto-generated constructor stub

}

SystemUI::~SystemUI() {
	// TODO Auto-generated destructor stub
}

void SystemUI::event(systemUIEvent e) {
	switch (e) {
		case (systemUIEvent::MODE_SW_PRESSED):
			switchMode();
			break;

		case (systemUIEvent::UP_SW_PRESSED):
			break;

		case (systemUIEvent::DOWN_SW_PRESSED):
			break;

		case (systemUIEvent::SELECT_SW_PRESSED):
			break;
	}
}

OperationMode SystemUI::getOperationMode() {
	return mode;
}
