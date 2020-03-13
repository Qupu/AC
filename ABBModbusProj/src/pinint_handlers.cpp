/*
 * pinint_handlers.cpp
 *
 *  Created on: 10 Mar 2020
 *      Author: Veli-Pekka Vaikkola
 */

#include "EventBuffer.h"
#include <atomic>
#include "SystemUI.h"

static const int pininterrupt_sw_interval = 20;
EventBuffer *ui_event_buffer;
volatile std::atomic_int pininterrupt_sw_counter;

#ifdef __cplusplus
extern "C" {
#endif
/* Handler for mode button */
void PIN_INT0_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0);

	if (ui_event_buffer && pininterrupt_sw_counter == 0) {
		pininterrupt_sw_counter = pininterrupt_sw_interval;

		if (!ui_event_buffer->full())
			ui_event_buffer->push(SystemUI::systemUIEvent::MODE_SW_PRESSED);
	}
}
/* Handler for up button */
void PIN_INT1_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH1);

	if (ui_event_buffer && pininterrupt_sw_counter == 0) {
		pininterrupt_sw_counter = pininterrupt_sw_interval;

		if (!ui_event_buffer->full())
			ui_event_buffer->push(SystemUI::systemUIEvent::DOWN_SW_PRESSED);
	}
}
/* Handler for down button */
void PIN_INT2_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH2);

	if (ui_event_buffer && pininterrupt_sw_counter == 0) {
		pininterrupt_sw_counter = pininterrupt_sw_interval;

		if (!ui_event_buffer->full())
			ui_event_buffer->push(SystemUI::systemUIEvent::UP_SW_PRESSED);
	}
}
/* Handler for power button */
void PIN_INT3_IRQHandler(void) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH3);
/*
	if (ui_event_buffer && pininterrupt_sw_counter == 0) {
		pininterrupt_sw_counter = pininterrupt_sw_interval;

		if (!ui_event_buffer->full())
			ui_event_buffer->push(SystemUI::systemUIEvent::POWER_SW_PRESSED);
	}
*/
}
#ifdef __cplusplus
}
#endif
