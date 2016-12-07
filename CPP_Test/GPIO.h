/*
 * GPIO.h
 *
 *  Created on: Nov 28, 2016
 *      Author: jcabral
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <msp430.h>
#include "Register/reg_t.hpp"

namespace MSP430{

enum GPIO_Port{
	P1,
	P2,
	P3
};

enum class GPIO_DIR{
	INPUT = 0,
	OUTPUT,
	DEFAULT = INPUT
};

enum class GPIO_RES{
	PULLDOW = 0,
	PULLUP,
	DISABLE,
	DEFAULT = DISABLE
};

enum class GPIO_INTERRUPT{
	LOW_TO_HIGH = 0,
	HIGH_TO_LOW,
	NONE
};

namespace implementation{

class GPIO_P1 {
public:
	using Input                 = reg_t<uint8_t, P1IN_>  ;
 	using Output    			= reg_t<uint8_t, P1OUT_> ;
 	using Direction				= reg_t<uint8_t, P1DIR_> ;
 	using Interrupt_Flag		= reg_t<uint8_t, P1IFG_> ;
 	using Interrupt_Edge_Select	= reg_t<uint8_t, P1IES_> ;
 	using Interrupt_Enable		= reg_t<uint8_t, P1IE_>  ;
 	using Port_Select			= reg_t<uint8_t, P1SEL_> ;
 	using Port_Select2			= reg_t<uint8_t, P1SEL2_>;
 	using Resistor_Enable		= reg_t<uint8_t, P1REN_> ;
};

class GPIO_P2 {
public:
	using Input                 = reg_t<uint8_t, P2IN_>  ;
	using Output    			= reg_t<uint8_t, P2OUT_> ;
	using Direction				= reg_t<uint8_t, P2DIR_> ;
	using Interrupt_Flag		= reg_t<uint8_t, P2IFG_> ;
	using Interrupt_Edge_Select	= reg_t<uint8_t, P2IES_> ;
	using Interrupt_Enable		= reg_t<uint8_t, P2IE_>  ;
	using Port_Select			= reg_t<uint8_t, P2SEL_> ;
	using Port_Select2			= reg_t<uint8_t, P2SEL2_>;
	using Resistor_Enable		= reg_t<uint8_t, P2REN_> ;
};

class GPIO_P3 {
public:
	using Input                 = reg_t<uint8_t, P3IN_>  ;
	using Output    			= reg_t<uint8_t, P3OUT_> ;
	using Direction				= reg_t<uint8_t, P3DIR_> ;
	using Port_Select			= reg_t<uint8_t, P3SEL_> ;
	using Port_Select2			= reg_t<uint8_t, P3SEL2_>;
	using Resistor_Enable		= reg_t<uint8_t, P3REN_> ;
};

template<class GPIO_Px, uint8_t PIN>
class GPIO_Base: public GPIO_Px{
public:
	GPIO_Base(){};

	GPIO_Base(GPIO_DIR dir, GPIO_RES res = GPIO_RES::DEFAULT)
	{
		set_dir(dir);
		set_res(res);
	};

	static void set(uint8_t val)
	{
		GPIO_Px::Output::write(static_cast<uint8_t>(val)<<PIN,1<<PIN);
	};

	static void clear()
	{
		set(0);
	};

	static void set_dir(GPIO_DIR dir)
	{
		GPIO_Px::Direction::write(static_cast<uint8_t>(dir)<<PIN,1<<PIN);
	};

	static void set_res(GPIO_RES res)
	{
		if(res==GPIO_RES::DISABLE || GPIO_Px::Direction::read() & 1<<PIN)	//If  pull up/down resistor is disable OR pin direction is out
			GPIO_Px::Resistor_Enable::clear(1<<PIN);						//Disable resistor
		else
		{
			GPIO_Px::Resistor_Enable::set(1<<PIN);							//Enable resistor
			GPIO_Px::Output::write(static_cast<uint8_t>(res)<<PIN, 1<<PIN);	//set mode
		}
	};

	static bool read()
	{
		return(GPIO_Px::Input::read() & (1<<PIN));
	};

	static void set_pin_function(bool sel1, bool sel2)
	{
		GPIO_Px::Port_Select::write(sel1<<PIN);
		GPIO_Px::Port_Select2::write(sel2<<PIN);
	}

	static void unused_pins(uint8_t mask)
	{
		GPIO_Px::Direction::set(mask,mask);
	}
};

template<class GPIO_Px, uint8_t PIN>
class GPIO_Interrupts: public GPIO_Base<GPIO_Px,PIN>{
public:
	GPIO_Interrupts(GPIO_DIR dir, GPIO_RES res = GPIO_RES::DEFAULT ):
		GPIO_Base<GPIO_Px, PIN>(dir,res){};

	void set_interrupt(GPIO_INTERRUPT val)
	{
		if (val== GPIO_INTERRUPT::NONE)
			GPIO_Px::Interrupt_Enable::clear(1<<PIN);
		else
		{
			GPIO_Px::Interrupt_Enable::set(1<<PIN);
			GPIO_Px::Interrupt_Edge_Select::write(static_cast<uint8_t>(val)<<PIN,1<<PIN);
		}
	};
	bool check_interrupt_flag()
	{
		return GPIO_Px::Interrupt_Flag::read() & 1<<PIN;
	}
	void clear_interrupt_flag()
	{
		GPIO_Px::Interrupt_Flag::clear(1<<PIN);
	};
};

}


template<GPIO_Port GPIO_P, uint8_t PIN>
//	class GPIO: public  implementation::GPIO_Base<GPIO_P,PIN>{
	class GPIO{
		GPIO(GPIO_DIR dir, GPIO_RES res){};
};

template<uint8_t PIN>
class GPIO<P1,PIN>: public implementation::GPIO_Interrupts<implementation::GPIO_P1,PIN>{
public:
	GPIO(GPIO_DIR dir,GPIO_RES res): implementation::GPIO_Interrupts<implementation::GPIO_P1,PIN>(dir,res){};
};

template<uint8_t PIN>
class GPIO<P2,PIN>: public implementation::GPIO_Interrupts<implementation::GPIO_P2,PIN>{
public:
	GPIO(GPIO_DIR dir,GPIO_RES res):implementation::GPIO_Interrupts<implementation::GPIO_P2,PIN>(dir,res){};
};

template<uint8_t PIN>
class GPIO<P3,PIN>: public implementation::GPIO_Base<implementation::GPIO_P3,PIN>{
public:
	GPIO(GPIO_DIR dir,GPIO_RES res):implementation::GPIO_Base<implementation::GPIO_P3,PIN>(dir,res){};
};

}

#endif
