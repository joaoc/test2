#include <msp430.h> 
#include "GPIO.h"
/*
 * main.cpp
 */
using namespace MSP430;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    GPIO<GPIO_Port::P3,2> LED(GPIO_DIR::OUTPUT, GPIO_RES::DISABLE);
    LED.set(1);

    while(1){};

	return 0;
}
