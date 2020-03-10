/*
 * SystemMenu.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef SYSTEMUI_H_
#define SYSTEMUI_H_

#include "SimpleMenu.h"
#include "MenuItem.h"
#include "LiquidCrystal.h"

enum class OperationMode {AUTOMATIC, MANUAL};

class SystemUI {
public:
	const int lcdWidth = 16;
	const int lcdHeight = 2;

	enum class systemUIEvent {
			UP_SW_PRESSED,
			DOWN_SW_PRESSED,
			SELECT_SW_PRESSED,
			MODE_SW_PRESSED,
			SHOW
			// More Events?
		};
	SystemUI();
	virtual ~SystemUI();
	void event(systemUIEvent e);
	OperationMode getOperationMode();
	double getPressure();
	int getFrequency();

private:
	LiquidCrystal *lcd;

	SimpleMenu autoModeMenu;
	SimpleMenu manualModeMenu;
	SimpleMenu *currMenu;

	DecimalEdit *pressureItem;
	IntegerEdit *frequencyItem;

	void switchMode();

	OperationMode mode;
};

#endif /* SYSTEMUI_H_ */
