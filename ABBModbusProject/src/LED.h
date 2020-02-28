
#ifndef LED_H_
#define LED_H_

#include <string>
#include "ITMwrapper.h"
#include "digitalIOPin.h"

class LED {
public:
	LED(int _port, int _pin);
	virtual ~LED();
	void turnOn() volatile;
	void turnOff() volatile;
	virtual void toggle() volatile;
	bool on_p() volatile;

private:
	volatile DigitalIOPin pin;

protected:
	volatile bool state;
};

#endif /* LED_H_ */
