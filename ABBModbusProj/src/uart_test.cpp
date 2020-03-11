/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include "DigitalIoPin.h"
#include "SDPSensor.h"

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#include <cstring>
#include <cstdio>

#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "LpcUart.h"
#include "DigitalIoPin.h"
#include "SDPSensor.h"
#include "Switch.h"
#include "SystemUI.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/


/*****************************************************************************
 * Public functions
 ****************************************************************************/
static volatile int counter;
static volatile uint32_t systicks;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	systicks++;
	if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}

#if 0
void printRegister(ModbusMaster& node, uint16_t reg)
{
	uint8_t result;
	// slave: read 16-bit registers starting at reg to RX buffer
	result = node.readHoldingRegisters(reg, 1);

	// do something with data if read is successful
	if (result == node.ku8MBSuccess)
	{
		printf("R%d=%04X\n", reg, node.getResponseBuffer(0));
	}
	else {
		printf("R%d=???\n", reg);
	}
}

bool setFrequency(ModbusMaster& node, uint16_t freq)
{
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, freq); // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if(node.getResponseBuffer(0) & 0x0100) atSetpoint = true;
		}
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}


void abbModbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	printRegister(node, 3); // for debugging

	node.writeSingleRegister(0, 0x0406); // prepare for starting

	printRegister(node, 3); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printRegister(node, 3); // for debugging

	node.writeSingleRegister(0, 0x047F); // set drive to start mode

	printRegister(node, 3); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printRegister(node, 3); // for debugging

	int i = 0;
	int j = 0;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };

	while (1) {
		uint8_t result;

		// slave: read (2) 16-bit registers starting at register 102 to RX buffer
		j = 0;
		do {
			result = node.readHoldingRegisters(102, 2);
			j++;
		} while(j < 3 && result != node.ku8MBSuccess);
		// note: sometimes we don't succeed on first read so we try up to threee times
		// if read is successful print frequency and current (scaled values)
		if (result == node.ku8MBSuccess) {
			printf("F=%4d, I=%4d  (ctr=%d)\n", node.getResponseBuffer(0), node.getResponseBuffer(1),j);
		}
		else {
			printf("ctr=%d\n",j);
		}

		Sleep(3000);
		i++;
		if(i >= 20) {
			i=0;
		}
		// frequency is scaled:
		// 20000 = 50 Hz, 0 = 0 Hz, linear scale 400 units/Hz
		setFrequency(node, fa[i]);
	}
}
#else
bool setFrequency(ModbusMaster& node, uint16_t freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);

	Frequency = freq; // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	if(ctr>=20) printf("\rtimed out\n");
	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

void startter(ModbusMaster& node)
{
    node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

    ModbusRegister ControlWord(&node, 0);
    ModbusRegister StatusWord(&node, 3);
    ModbusRegister OutputFrequency(&node, 102);
    ModbusRegister Current(&node, 103);

    int state;

    while(state != 5) {

        state = 0;
        ControlWord = ControlWord | 0x8000;
        Sleep(5);

        switch(state)
        {
        case(0): //not ready to switch on
                ControlWord = ControlWord & 0xFFFE;
                StatusWord = StatusWord & 0xFFFE;

                ControlWord = ControlWord | 0x0006;

                Sleep(5);
                if(ControlWord & (1<<2) && ControlWord & (1<<1) && ControlWord & (0<<0)) state = 1;

        case(1): //ready to switch on
                StatusWord = StatusWord | 0x0001;

                ControlWord = ControlWord | 0x0007;

                Sleep(5);
                if(ControlWord & (1<<2) && ControlWord & (1<<1) && ControlWord & (1<<0)) state = 2;
        case(2): //ready to operte
                StatusWord = StatusWord | 0x0002;

                ControlWord = ControlWord | 0x0008;
                StatusWord = StatusWord | 0x1000;

                Sleep(5);
                if(ControlWord & (1<<3) && StatusWord & (1<<12)) state = 3;

        case(3): //operation enabled
                StatusWord = StatusWord | 0x0004;

                ControlWord = ControlWord | 0x0020;

                Sleep(5);
                if(ControlWord & (1<<5)) state = 4;

        case(4): //RFG: ACCELERATORENABLED
                ControlWord = ControlWord | 0x0040;

                Sleep(5);
                if(ControlWord & (1<<6)) state = 5;

        case(5): //operating
                StatusWord = StatusWord | 0x0080;
                Sleep(5);
        }
    }
}

void abbModbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);


	// need to use explicit conversion since printf's variable argument doesn't automatically convert this to an integer
	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x0406; // prepare for starting

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	ControlWord = 0x047F; // set drive to start mode

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printf("Status=%04X\n", (int)StatusWord); // for debugging

	int i = 0;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000, 10000, 15000, 20000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000, 1000 };

	while (1) {

		// just print the value without checking if we got a -1
		printf("F=%4d, I=%4d\n", (int) OutputFrequency, (int) Current);

		Sleep(3000);
		i++;
		if(i >= 20) {
			i=0;
		}
		// frequency is scaled:
		// 20000 = 50 Hz, 0 = 0 Hz, linear scale 400 units/Hz
		setFrequency(node, fa[i]);
	}
}
#endif


void modbusTest()
{
	ModbusMaster node(2); // Create modbus object that connects to slave id 2


	while (1) {
		static uint32_t i;
		uint8_t j, result;
		uint16_t data[6];

		for(j = 0; j < 6; j++) {
			i++;
			// set word(j) of TX buffer to least-significant word of counter (bits 15..0)
			node.setTransmitBuffer(j, i & 0xFFFF);
		}
		// slave: write TX buffer to (6) 16-bit registers starting at register 0
		result = node.writeMultipleRegisters(0, j);

		// slave: read (6) 16-bit registers starting at register 2 to RX buffer
		result = node.readHoldingRegisters(2, 6);

		// do something with data if read is successful
		if (result == node.ku8MBSuccess)
		{
			for (j = 0; j < 6; j++)
			{
				data[j] = node.getResponseBuffer(j);
			}
			printf("%6d, %6d, %6d, %6d, %6d, %6d\n", data[0], data[1], data[2], data[3], data[4], data[5]);
		}
		Sleep(1000);
	}
}

// Is used to detect the current operation mode for te fan:
// OperationMode::AUTOMATIC   - the fan system is in automatic mode
// OperationMode::MANUAL      - the fan system is in manual mode

/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif
	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart dbgu(cfg);

	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	Switch sw1(0,17,true,true,true);
	Switch sw2(1,11,true,true,true);

	Switch swe1(0,10,true,true,true);

	SDPSensor sdp(0x40);
	ModbusMaster node(2); // Create modbus object that connects to slave id 2
	startter(node);

	Board_LED_Set(0, false);
	Board_LED_Set(1, true);

	int freq = 0;

	OperationMode mode = OperationMode::MANUAL;

	double press;
	int setPressDiff = 10;

	SystemUI UI(true);
	UI.updateCurrPressure(50.0);
	char manualMode[30] = "MANUAL";
	char automaticMode[30] = "AUTOMATIC";
	while(1) {
		printf("\r %s\n", (UI.getOperationMode() == OperationMode::MANUAL? manualMode: automaticMode));
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());
		UI.event(SystemUI::systemUIEvent::DOWN_SW_PRESSED);
		printf("\rValue goes down\n");
		Sleep(2000);
		printf("\r %s\n", (UI.getOperationMode() == OperationMode::MANUAL? manualMode: automaticMode));
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());
		UI.event(SystemUI::systemUIEvent::DOWN_SW_PRESSED);
		printf("\rValue goes down\n");
		Sleep(2000);
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());

		printf("\rTurning off Power\n");
		UI.event(SystemUI::systemUIEvent::POWER_SW_PRESSED);
		Sleep(1000);
		UI.event(SystemUI::systemUIEvent::SHOW);
		Sleep(1000);

		printf("Trying to change the value down:");
		UI.event(SystemUI::systemUIEvent::DOWN_SW_PRESSED);
		Sleep(1000);
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());
		Sleep(1000);

		printf("\rTurning on Power\n");
		UI.event(SystemUI::systemUIEvent::POWER_SW_PRESSED);
		Sleep(1000);

		UI.event(SystemUI::systemUIEvent::MODE_SW_PRESSED);
		printf("\rMode Changes\n");
		Sleep(2000);
		printf("\r %s\n", (UI.getOperationMode() == OperationMode::MANUAL? manualMode: automaticMode));
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());
		UI.event(SystemUI::systemUIEvent::DOWN_SW_PRESSED);
		printf("\rValue goes down\n");
		Sleep(2000);
		printf("\r %s\n", (UI.getOperationMode() == OperationMode::MANUAL? manualMode: automaticMode));
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());
		Sleep(2000);
		UI.event(SystemUI::systemUIEvent::MODE_SW_PRESSED);
		printf("\rMode Changes\n");
		Sleep(2000);
		UI.event(SystemUI::systemUIEvent::UP_SW_PRESSED);
		printf("\rValue goes up\n");
		Sleep(2000);
		printf("\r %s\n", (UI.getOperationMode() == OperationMode::MANUAL? manualMode: automaticMode));
		printf("\rFREQUENCY: %2d PRESSURE: %.2f\n", UI.getTargetFrequency(), UI.getTargetPressure());
		Sleep(2000);
	}

	while(1)
	{
		switch (mode) {
			case (OperationMode::AUTOMATIC):
				if(press < setPressDiff-1) freq += 40;
				else if (press > setPressDiff+1) freq -= 40;

				if(sw1.read()) {
					printf("\rpressed sw1\n");
					setPressDiff += 1;

					while(sw1.read());
				}

				if(sw2.read())
				{
					printf("\rpressed s2\n");

					setPressDiff -= 1;

					while(sw2.read());
				}

				if(swe1.read()) {
					mode = OperationMode::MANUAL;
					printf("\rautomode off\n");
					while(swe1.read());
				}

				printf("\rpressdiff %d\n",setPressDiff);

			break;

			case (OperationMode::MANUAL):
				if(sw1.read()) {
					printf("\rpressed sw1\n");
					freq += 2000;

					while(sw1.read());
				}

				if(sw2.read())
				{
					printf("/rpressed s2\n");
					freq -= 2000;

					while(sw2.read());
				}

				if(swe1.read()) {
					mode = OperationMode::MANUAL;
					printf("\rautomode on\n");
					while(swe1.read());
				}

				break;
		}

		sdp.ReadPressure(press);
		setFrequency(node,freq);

		printf("\r%.2f",press);
		printf("\r%d\n",freq);
	}

	abbModbusTest();

	return 1;
}
