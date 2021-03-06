/*
 * SystemMenu.h
 *
 *  Created on: 9.3.2020
 *      Author: rqqht
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include "SimpleMenu.h"
#include "MenuItem.h"
#include "LiquidCrystal.h"
#include "PressureEdit.h"
#include "FrequencyEdit.h"

enum class OperationMode {AUTOMATIC, MANUAL};
enum class ErrorStatus {NO_ERROR, TARGET_PRESSURE_UNREACHABLE_ERROR};

class SystemManager {
public:
	static const int lcdWidth = 16;
	static const int lcdHeight = 2;

	enum class SystemEvent {
			UP_SW_PRESSED,
			DOWN_SW_PRESSED,
			SELECT_SW_PRESSED,	// currently unused
			MODE_SW_PRESSED,
			SHOW,
			POWER_SW_PRESSED,
			TARGET_PRESSURE_LATENCY_ERROR,
			ERROR_ACK
		};
	SystemManager(bool _powerOn);
	virtual ~SystemManager();
	void event(SystemEvent e);
	OperationMode getOperationMode();
	void setOperationMode(OperationMode _mode);
	double getTargetPressure();
	int getTargetFrequency();
	void updateCurrPressure(double _currPressure);
	ErrorStatus getErrorStatus() { return errorStatus; }

private:
	// LiquidCrystal data:
	LiquidCrystal *lcd;
	DigitalIoPin RS;
	DigitalIoPin EN;
	DigitalIoPin D0;
	DigitalIoPin D1;
	DigitalIoPin D2;
	DigitalIoPin D3;

	// Menu data:
	SimpleMenu autoModeMenu;
	SimpleMenu manualModeMenu;
	SimpleMenu *currMenu;
	PressureEdit *pressureEdit;
	FrequencyEdit *frequencyEdit;
	double currPressure;

	// General status variables:
	OperationMode mode;
	bool powerOn;
	ErrorStatus errorStatus;

	void switchMode();
	void displayPowerOff();
	void displayLatencyError();
};

#endif /* SYSTEMMANAGER_H_ */
