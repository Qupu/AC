#ifndef _DIGITALIOPIN_H_
#define _DIGITALIOPIN_H_

class DigitalIOPin {

public:
    DigitalIOPin(int _port, int _pin, bool _input = true, bool _pullup = true, bool _invert = false);
    virtual ~DigitalIOPin() {};
    bool read() volatile;
    void write(bool value) volatile;
 
private:
	volatile int port;
	volatile int pin;
	volatile int input;
	volatile bool invert;
};

#endif
