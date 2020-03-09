/*
 * SystemMenu.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef SYSTEMUI_H_
#define SYSTEMUI_H_

#include "SimpleMenu.h"
#include "LiquidCrystal.h"
#include "BarGraph.h"

enum class OperationMode {AUTOMATIC, MANUAL};

class SystemUI {
public:
	SystemMenu();
	virtual ~SystemMenu();
	void event(MenuItem::menuEvent e);
	OperationMode getOperationMode();
	double getPressure();
	int getFrequency();

private:
	BarGraph barGraph;
	LiquidCrystal *lcd;

	SimpleMenu autoModeMenu;
	SimpleMenu manualModeMenu;
	SimpleMenu *currMenu;
	void switchMode();

	OperationMode mode;
};

#endif /* SYSTEMUI_H_ */
