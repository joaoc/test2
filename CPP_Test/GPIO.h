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

namespace implementation{

class GPIO_P1 {
public:
	GPIO_P1()
 		: Input(				*reinterpret_cast<uint8_t*>(P1IN)	)
 	    , Output(				*reinterpret_cast<uint8_t*>(P1OUT)	)
 	    , Direction(			*reinterpret_cast<uint8_t*>(P1DIR)	)
 	    , Interrupt_Flag(		*reinterpret_cast<uint8_t*>(P1IFG)	)
 	    , Interrupt_Edge_Select(*reinterpret_cast<uint8_t*>(P1IES)	)
 	    , Interrupt_Enable(		*reinterpret_cast<uint8_t*>(P1IE)	)
 	    , Port_Select(			*reinterpret_cast<uint8_t*>(P1SEL)	)
 	    , Port_Select2(			*reinterpret_cast<uint8_t*>(P1SEL2)	)
 	    , Resistor_Enable(		*reinterpret_cast<uint8_t*>(P1REN)	)
		{}

	volatile uint8_t& Input;
 	volatile uint8_t& Output;
 	volatile uint8_t& Direction;
 	volatile uint8_t& Interrupt_Flag;
 	volatile uint8_t& Interrupt_Edge_Select;
 	volatile uint8_t& Interrupt_Enable;
 	volatile uint8_t& Port_Select;
 	volatile uint8_t& Port_Select2;
 	volatile uint8_t& Resistor_Enable;
};

class GPIO_P2 {
public:
	GPIO_P2()
 		: Input(				*reinterpret_cast<uint8_t*>(P2IN)	)
 	    , Output(				*reinterpret_cast<uint8_t*>(P2OUT)	)
 	    , Direction(			*reinterpret_cast<uint8_t*>(P2DIR)	)
 	    , Interrupt_Flag(		*reinterpret_cast<uint8_t*>(P2IFG)	)
 	    , Interrupt_Edge_Select(*reinterpret_cast<uint8_t*>(P2IES)	)
 	    , Interrupt_Enable(		*reinterpret_cast<uint8_t*>(P2IE)	)
 	    , Port_Select(			*reinterpret_cast<uint8_t*>(P2SEL)	)
 	    , Port_Select2(			*reinterpret_cast<uint8_t*>(P2SEL2)	)
 	    , Resistor_Enable(		*reinterpret_cast<uint8_t*>(P2REN)	)
		{}

	volatile uint8_t& Input;
 	volatile uint8_t& Output;
 	volatile uint8_t& Direction;
 	volatile uint8_t& Interrupt_Flag;
 	volatile uint8_t& Interrupt_Edge_Select;
 	volatile uint8_t& Interrupt_Enable;
 	volatile uint8_t& Port_Select;
 	volatile uint8_t& Port_Select2;
 	volatile uint8_t& Resistor_Enable;
};

class GPIO_P3 {
public:
	GPIO_P3()
 		: Input(				*reinterpret_cast<uint8_t*>(P3IN)	)
 	    , Output(				*reinterpret_cast<uint8_t*>(P3OUT)	)
 	    , Direction(			*reinterpret_cast<uint8_t*>(P3DIR)	)
 	    , Port_Select(			*reinterpret_cast<uint8_t*>(P3SEL)	)
 	    , Port_Select2(			*reinterpret_cast<uint8_t*>(P3SEL2)	)
 	    , Resistor_Enable(		*reinterpret_cast<uint8_t*>(P3REN)	)
		{}

	volatile uint8_t& Input;
 	volatile uint8_t& Output;
 	volatile uint8_t& Direction;
 	volatile uint8_t& Port_Select;
 	volatile uint8_t& Port_Select2;
 	volatile uint8_t& Resistor_Enable;
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

	void set(uint8_t val)
	{
		if(val)
			this->Output|=1<<PIN;
		else
			this->Output&=!(1<<PIN);
	};

	void clear()
	{
		set(0);
	};

	void set_dir(GPIO_DIR dir)
	{
		if(dir == GPIO_DIR::INPUT)
			this->Direction&=!(1<<PIN);
		else //output
		{
			set_res(GPIO_RES::DISABLE);
			this->Direction|=(1<<PIN);
		}
	};

	void set_res(GPIO_RES res)
	{
		if(res == GPIO_RES::DISABLE)
		{
			this->Resistor_Enable&=!(1<<PIN);
		}
		else
		{
			if(res == GPIO_RES::PULLDOW)
				this->Output&=!(1<<PIN);
			else
				this->Output|=(1<<PIN);
		}
	};

	bool read()
	{
		return(this->Input & (1<<PIN));
	};
};

template<class GPIO_Px, uint8_t PIN>
class GPIO_Interrupts: public GPIO_Base<GPIO_Px,PIN>{
public:
	GPIO_Interrupts(GPIO_DIR dir, GPIO_RES res = GPIO_RES::DEFAULT ):
		GPIO_Base<GPIO_Px, PIN>(dir,res){};

	void set_interrupt(uint8_t val){};
	void set_interrupt_edge(){};
	bool check_interrupt_flag();
	void clear_interrupt_flag(){};
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
