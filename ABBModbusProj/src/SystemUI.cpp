/*
 * SystemMenu.cpp
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#include "SystemUI.h"

// Private Methods:
void SystemUI::switchMode() {
	switch (mode) {
		case (OperationMode::AUTOMATIC):
			mode = OperationMode::MANUAL;
			currMenu = &ManualModeMenu;
			break;
		case (OperationMode::MANUAL):
			mode = OperationMode::AUTOMATIC;
			currMenu = &AutomaticModeMenu;
			break;
	}


}

// ---------------------
// The public interface:

SystemUI::SystemMenu() {
	// TODO Auto-generated constructor stub

}

SystemUI::~SystemMenu() {
	// TODO Auto-generated destructor stub
}

void SystemUI::event(MenuItem::menuEvent e) {

}

OperationMode SystemUI::getOperationMode() {
	return mode;
}
