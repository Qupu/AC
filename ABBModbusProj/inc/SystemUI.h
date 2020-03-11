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
#include "PressureEdit.h"
#include "FrequencyEdit.h"

enum class OperationMode {AUTOMATIC, MANUAL};

class SystemUI {
public:
	static const int lcdWidth = 16;
	static const int lcdHeight = 2;

	enum class systemUIEvent {
			UP_SW_PRESSED,
			DOWN_SW_PRESSED,
			SELECT_SW_PRESSED,
			MODE_SW_PRESSED,
			SHOW,
			POWER_SW_PRESSED
			// More Events?
		};
	SystemUI(bool _powerOn);
	virtual ~SystemUI();
	void event(systemUIEvent e);
	OperationMode getOperationMode();
	double getTargetPressure();
	int getTargetFrequency();
	void updateCurrPressure(double _currPressure);

private:
	LiquidCrystal *lcd;

	DigitalIoPin RS;
	DigitalIoPin EN;
	DigitalIoPin D0;
	DigitalIoPin D1;
	DigitalIoPin D2;
	DigitalIoPin D3;

	SimpleMenu autoModeMenu;
	SimpleMenu manualModeMenu;
	SimpleMenu *currMenu;

	PressureEdit *pressureEdit;
	FrequencyEdit *frequencyEdit;

	double currPressure;

	void switchMode();
	void displayPowerOff();

	OperationMode mode;
	bool powerOn;
};

#endif /* SYSTEMUI_H_ */
