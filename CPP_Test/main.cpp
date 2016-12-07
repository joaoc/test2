#include <msp430.h> 
#include "GPIO.h"
/*
 * main.cpp
 */
using namespace MSP430;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    GPIO<GPIO_Port::P3,2> BLUE_LED(GPIO_DIR::OUTPUT, GPIO_RES::DISABLE);
    BLUE_LED.set(1);

    GPIO<GPIO_Port::P3,4> Presence_LED(GPIO_DIR::OUTPUT, GPIO_RES::DISABLE);
    Presence_LED.set(1);

    GPIO<GPIO_Port::P1,6> Call_BT(GPIO_DIR::INPUT, GPIO_RES::PULLUP);

    while(1){
    	BLUE_LED.set(Call_BT.read());

    	//_no_operation();
    };

	return 0;
}
