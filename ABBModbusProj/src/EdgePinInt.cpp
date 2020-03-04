/*
 * EdgePinInt.cpp
 *
 *  Created on: 29 Feb 2020
 *      Author: Veli-Pekka Vaikkola
 */

#include "EdgePinInt.h"
#include "chip.h"

EdgePinInt::EdgePinInt(int int_nr, int port, int pin, bool high) {
	if (instances == 0) {
		// Initialize pin interrupt hardware
		Chip_PININT_Init(LPC_GPIO_PIN_INT);

		// Configure and enable pin interrupt in the pin interrupt hardware
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);
		Chip_SYSCTL_PeriphReset(RESET_PININT);
	}

	++instances;

	// Configure pin as inverted input with pullup
	Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
			(IOCON_DIGMODE_EN | IOCON_INV_EN | IOCON_MODE_PULLUP | IOCON_MODE_INACT));

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);

	// Configure pin interrupt to generate interrupt when it sees a rising edge on pin
	Chip_INMUX_PinIntSel(int_nr, port, pin);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(int_nr));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(int_nr));

	if (high) {
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(int_nr));
	} else {
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(int_nr));
	}

	// Enable pin interrupt in NVIC
	NVIC_ClearPendingIRQ((IRQn) (int_nr + 7));
	NVIC_EnableIRQ((IRQn) (int_nr + 7));
}

EdgePinInt::~EdgePinInt() {
	--instances;
}
int EdgePinInt::instances = 0;
