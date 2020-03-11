/*
 * EventBuffer.h
 *
 *  Created on: 10 Mar 2020
 *      Author: Veli-Pekka Vaikkola
 */

#ifndef EVENTBUFFER_H_
#define EVENTBUFFER_H_

#include "SystemUI.h"
#include "board.h"

class EventBuffer {
public:
	EventBuffer(uint32_t capacity_ = 5);
	virtual ~EventBuffer();
	void push(SystemUI::systemUIEvent e);
	SystemUI::systemUIEvent shift();
	uint32_t size();
	bool full();
	bool empty();

private:
	uint32_t capacity;

	volatile SystemUI::systemUIEvent * buffer;
	uint32_t read;
	volatile uint32_t write;

	uint32_t mask(uint32_t x);
};

#endif /* EVENTBUFFER_H_ */
