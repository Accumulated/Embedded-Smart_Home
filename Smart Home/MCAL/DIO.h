
#ifndef MCAL_DIO_H_
#define MCAL_DIO_H_

#include "../types.h"
#include "../macros.h"

#define F_CPU 8000000ul




/* Define DDRA_REG, PORTA_REG, PIN_REG address from data sheet for manual control over it*/
/* Local declaration so it's in c file not h file*/
#define PORTA_REG		*((volatile u8*)0x3B)
#define DDRA_REG		*((volatile u8*)0x3A)
#define PINA_REG		*((volatile u8*)0x39)

#define PORTB_REG		*((volatile u8*)0x38)
#define DDRB_REG		*((volatile u8*)0x37)
#define PINB_REG		*((volatile u8*)0x36)

#define PORTC_REG		*((volatile u8*)0x35)
#define DDRC_REG		*((volatile u8*)0x34)
#define PINC_REG		*((volatile u8*)0x33)

#define PORTD_REG		*((volatile u8*)0x32)
#define DDRD_REG		*((volatile u8*)0x31)
#define PIND_REG		*((volatile u8*)0x30)



/* PORTx in AVR corresponding to numbers*/
#define PORTA	0
#define PORTB	1
#define PORTC	2
#define PORTD	3

/* DDR DIRECTION INDECATION for PORTx*/
#define INPUT	0
#define	OUTPUT	1

// AVR PORTS DIRECTION CONTROL
void DIO_vidSetPortxDir( u8 PortName, u8 PortDir);
void DIO_vidSetPinxDir( u8 portname, u8 pin_number, u8 state);

// PORT_REGISTER CONTROL
void DIO_vidSetPortBitValue(u8 portname, u8 port_number, u8 state);
void DIO_vidSetPortValue( u8 Portname, u8 Value);
u8 DIO_GetPortValue(u8 portname);
u8 DIO_u8GetPortBitValue(u8 portname, u8 Pin_number);


// PIN_REGISTER CONTROL
void DIO_vidSetPinValue( u8 portname, u8 value);
u8 DIO_u8GetPinBitValue( u8 Portname, u8 Pin_number);
void DIO_vidSetPintBitValue(u8 portname, u8 port_number, u8 state);
u8 DIO_GetPinValue(u8 portname);


#endif /* MCAL_DIO_H_ */
