
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "LED.h"
#include "DigitalIOPin.h"

LED::LED(int _port, int _pin): pin(_port, _pin, false, false, true) {
	state = false;
	pin.write(false);
}

LED::~LED() {
	// TODO Auto-generated destructor stub
}

void LED::turnOn() volatile {
	if (state == false) {
		state = true;
		pin.write(true);
	}
}

void LED::turnOff() volatile {
	if (state == true) {
		state = false;
		pin.write(false);
	}
}

void LED::toggle() volatile {
	state = !state;
	pin.write(state);
}

bool LED::on_p() volatile {
	return state;
}

