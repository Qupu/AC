
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "DigitalIOPin.h"

DigitalIOPin::DigitalIOPin(int _port, int _pin, bool _input, bool _pullup, bool _invert) :
  port(_port), pin(_pin), input(_input), invert(_invert) {
	if (_input) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, ((_pullup? IOCON_MODE_PULLUP: 0x0) | IOCON_DIGMODE_EN | (_invert? IOCON_INV_EN : 0x0)));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	else {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_DIGMODE_EN);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
}

bool DigitalIOPin::read() volatile {
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void DigitalIOPin::write(bool value) volatile {
	if (!input)
		Chip_GPIO_SetPinState(LPC_GPIO, port, pin, value != invert);
}



