#ifndef _DigitalIoPin_H_
#define _DigitalIoPin_H_

class DigitalIoPin {

public:
    DigitalIoPin(int _port, int _pin, bool _input = true, bool _pullup = true, bool _invert = false);
    virtual ~DigitalIoPin() {};
    bool read() volatile;
    void write(bool value) volatile;
 
protected:
	volatile int port;
	volatile int pin;
	volatile bool input;
	volatile bool invert;
};

#endif
