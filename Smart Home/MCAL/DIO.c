/*
 * DIO.c
 *
 *  Created on: Aug 17, 2019
 *      Author: Newname
 */

#include "DIO.h"




/*
 *	0 -> PORTA
 *	1 -> PORTB
 *	2 -> PORTC
 *	3 -> PORTC
 *
 *	0 -> INPUT
 *	1 -> OUTPUT
 */

/* PORTx Direction only*/
void DIO_vidSetPortxDir(u8 PortName, u8 PortDir)
{
	switch(PortName)
	{
		// PORTA in AVR MICROCONTROLLER
		case PORTA:
			if(PortDir == INPUT)
			{
				// DDR for register is all input directed
				DDRA_REG = 0x00;
			}
			else
			{
				// DDR for register is all output directed
				DDRA_REG = 0xff;
			}
			break;

		// PORTB in AVR MICROCONTROLLER
		case PORTB:
			if(PortDir == INPUT)
			{
				// DDR for register is all input directed
				DDRB_REG = 0x00;
			}
			else
			{
				// DDR for register is all output directed
				DDRB_REG = 0xff;
			}
			break;

		// PARTC in AVR MICROCONTROLLER
		case PORTC:
			if(PortDir == INPUT)
			{
				// DDR for register is all input directed
				DDRC_REG = 0x00;
			}
			else
			{
				// DDR for register is all output directed
				DDRC_REG = 0xff;
			}
			break;

		// PORTD in AVR MICROCONTROLLER
		case PORTD:
			if(PortDir == INPUT)
			{
				// DDR for register is all input directed
				DDRD_REG = 0x00;
			}
			else
			{
				// DDR for register is all output directed
				DDRD_REG = 0xff;
			}
			break;

		default:
			break;
	}
}

// PORTx VALUE
void DIO_vidSetPortValue(u8 Portname, u8 Value)
{
	switch(Portname)
	{
		case PORTA:
			PORTA_REG = Value;
			break;
		case PORTB:
			PORTB_REG = Value;
			break;
		case PORTC:
			PORTC_REG = Value;
			break;
		case PORTD:
			PORTD_REG = Value;
			break;
		default:
			break;
	}
}

// PORTx.n Direction
void DIO_vidSetPinxDir(u8 portname, u8 pin_number, u8 state)
{
	switch(portname)
	{
		case PORTA:
			if (state == OUTPUT)
				set_bit(DDRA_REG, pin_number);
			else if(state == INPUT)
			{
				// DDR IS INPUT
				clr_bit(DDRA_REG, pin_number);
				// PORT IS PULLED UP
				set_bit(PORTA_REG, pin_number);
			}
			else;
			break;

		case PORTB:
			if (state == OUTPUT)
				set_bit(DDRB_REG, pin_number);
			else if(state == INPUT)
			{
				clr_bit(DDRB_REG, pin_number);
				// PORT IS PULLED UP
				set_bit(PORTB_REG, pin_number);
			}
			else;
			break;

		case PORTC:
			if (state == OUTPUT)
				set_bit(DDRC_REG, pin_number);
			else if(state == INPUT)
			{
				clr_bit(DDRC_REG, pin_number);
				// PORT IS PULLED UP
				set_bit(PORTC_REG, pin_number);
			}
			else;
			break;

		case PORTD:
			if (state == OUTPUT)
				set_bit(DDRD_REG, pin_number);
			else if(state == INPUT)
			{
				// CLEAR
				clr_bit(DDRD_REG, pin_number);
				// PORT IS PULLED UP
				set_bit(PORTD_REG, pin_number);
			}
			else;
			break;

		default:
			break;
	}
}

// PINx Whole value
void DIO_vidSetPinValue(u8 portname, u8 value)
{
	switch(portname)
	{
		case PORTA:
			PINA_REG = value;
			break;
		case PORTB:
			PINB_REG = value;
			break;
		case PORTC:
			PINC_REG = value;
			break;
		case PORTD:
			PIND_REG = value;
			break;
		default:
			break;
	}
}

// GET PIN VALUE
u8 DIO_u8GetPinBitValue(u8 Portname, u8 Pin_number)
{
	u8 tmp = 0;
	switch(Portname)
	{
		case PORTA:
			tmp = PINA_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		case PORTB:
			tmp = PINB_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		case PORTC:
			tmp = PINC_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		case PORTD:
			tmp = PIND_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		default:
			break;
	}
	return tmp;
}

// PORTx Bit value
void DIO_vidSetPortBitValue(u8 portname, u8 port_number, u8 state)
{
	switch(portname)
	{
		case PORTA:
			if (state == OUTPUT)
				set_bit(PORTA_REG, port_number);
			else if(state == INPUT)
				clr_bit(PORTA_REG, port_number);
			else;
			break;

		case PORTB:
			if (state == OUTPUT)
				set_bit(PORTB_REG, port_number);
			else if(state == INPUT)
				clr_bit(PORTB_REG, port_number);
			else;
			break;

		case PORTC:
			if (state == OUTPUT)
				set_bit(PORTC_REG, port_number);
			else if(state == INPUT)
				clr_bit(PORTC_REG, port_number);
			else;
			break;

		case PORTD:
			if (state == OUTPUT)
				set_bit(PORTD_REG, port_number);
			else if(state == INPUT)
				clr_bit(PORTD_REG, port_number);
			else;
			break;

		default:
			break;
	}
}



// PINx Bit value
void DIO_vidSetPintBitValue(u8 portname, u8 port_number, u8 state)
{
	switch(portname)
	{
		case PORTA:
			if (state == OUTPUT)
				set_bit(PINA_REG, port_number);
			else if(state == INPUT)
				clr_bit(PINA_REG, port_number);
			else;
			break;

		case PORTB:
			if (state == OUTPUT)
				set_bit(PINB_REG, port_number);
			else if(state == INPUT)
				clr_bit(PINB_REG, port_number);
			else;
			break;

		case PORTC:
			if (state == OUTPUT)
				set_bit(PINC_REG, port_number);
			else if(state == INPUT)
				clr_bit(PINC_REG, port_number);
			else;
			break;

		case PORTD:
			if (state == OUTPUT)
				set_bit(PIND_REG, port_number);
			else if(state == INPUT)
				clr_bit(PIND_REG, port_number);
			else;
			break;

		default:
			break;
	}
}

u8 DIO_GetPortValue(u8 portname)
{
	u8 tmp = 0;
	switch(portname)
	{
		case PORTA:
			tmp = PORTA_REG;
			break;

		case PORTB:
			tmp = PORTB_REG;
			break;

		case PORTC:
			tmp = PORTC_REG;
			break;

		case PORTD:
			tmp = PORTD_REG;
			break;

		default:
			break;
	}
	return tmp;
}

u8 DIO_GetPinValue(u8 portname)
{
	u8 value = 0;
	switch(portname)
	{
		case PORTA:
			value = PINA_REG;
			break;

		case PORTB:
			value = PINB_REG;
			break;

		case PORTC:
			value = PINC_REG;
			break;

		case PORTD:
			value = PIND_REG;
			break;

		default:
			break;
	}
	return value;
}

u8 DIO_u8GetPortBitValue(u8 portname, u8 Pin_number)
{
	u8 tmp = 0;
	switch(portname)
	{
		case PORTA:
			tmp = PORTA_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		case PORTB:
			tmp = PORTB_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		case PORTC:
			tmp = PORTC_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		case PORTD:
			tmp = PORTD_REG;
			tmp = get_bit(tmp,Pin_number);
			break;

		default:
			break;
	}
	return tmp;
}
