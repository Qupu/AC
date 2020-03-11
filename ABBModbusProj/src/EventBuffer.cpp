/*
 * EventBuffer.cpp
 *
 *  Created on: 10 Mar 2020
 *      Author: Veli-Pekka Vaikkola
 */

#include <EventBuffer.h>
#include <cassert>


EventBuffer::EventBuffer(uint32_t capacity_) : read(0), write(0) {
	capacity = (capacity_ >= 32) ? 31 : capacity_;
	buffer = new SystemUI::systemUIEvent[(1UL << capacity)];
}

EventBuffer::~EventBuffer() {
	delete[] buffer;
}

void EventBuffer::push(SystemUI::systemUIEvent e) {
	assert(!full());
	buffer[mask(write++)] = e;
}

SystemUI::systemUIEvent EventBuffer::shift() {
	assert(!empty());
	return buffer[mask(read++)];
}

uint32_t EventBuffer::size() {
	return write - read;
}

bool EventBuffer::full() {
	return size() == capacity;
}

bool EventBuffer::empty() {
	return read == write;
}

uint32_t EventBuffer::mask(uint32_t x) {
	return x & (capacity - 1);
}
