/*
 * LCD.c
 *
 *  Created on: Aug 23, 2019
 *      Author: Newname
 */

#define F_CPU 8000000ul
#include "LCD.h"
#include "LCD_CFG.h"
#include "../MCAL/DIO.h"
#include "util/delay.h"
#include <stdlib.h>

#include "../EMEMBERS.h"



u8 DATA = 0;

/*
	write_operation 	0
	read_operation		1
 	 IR			0
	 DR			1
  */


// INITIALIZATION, always start an LCD with
void LCD_vidInitialize(void)
{
	// Set PORTD, PORTC as output by default
	DIO_vidSetPortxDir(LCD_PORT_DATA, 1);	// PORTC
	DIO_vidSetPortxDir(LCD_PORT_CONTROL, 1);	// PORTD

	// RS and RW = 0
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RS_PIN, 0);
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RW_PIN, 0);


	// Function Set, F = 0, DL = 1, N = 1
	LCD_vidInstruction(0X38, write_operation, IR);

	// Delay > 39us
	_delay_us(11);

	// Display ON/OFF Control D = 1, C = 1, B = 0
	LCD_vidInstruction(0X0c, write_operation, IR);

	// Delay > 39us
	_delay_us(11);

	// Display Clear
	LCD_vidInstruction(0x01, write_operation, IR);

	// Delay > 1.53ms
	_delay_ms(2);

	// Entry Mode Set
	LCD_vidInstruction(0x06, write_operation, IR);
	_delay_us(11);

	LCD_String("welcome user");
	LCD_vidClearOnDemand(1, 32);
}


// Send instruction to execute
void LCD_vidInstruction(u8 command, u8 RW_value, u8 RS_value)
{
	// INDICATE WHICH REGISTER
	if(RS_value == IR)
	{
		// RS = 0, indication for Instruction Register
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RS_PIN, 0);
	}
	else
	{
		// RS = 1, indication for DATA Register
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RS_PIN, 1);
	}

	// INDICATE R/W OPERATION
	if(RW_value == write_operation)
	{
		// R/W = 0, write indication
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RW_PIN, 0);
		DIO_vidSetPortValue(LCD_PORT_DATA, command);
	}
	else
	{
		// R/W = 1, read indication
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RW_PIN, 1);
		// Change Direction of PORT to input
		DIO_vidSetPortxDir(LCD_PORT_DATA, 0);
	}

	// E = 1, H-to-L pulse
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, E_PIN, 1);

	// Wait to make enable wide
	_delay_us(10);

	// READ/WRITE VALUES FROM/TO LCD
	if(RW_value == write_operation)
	{

	}
	else
	{
		// Store the whole PIN_REG value
		DATA = DIO_GetPinValue(LCD_PORT_DATA);
	}

	// EN = 0
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, E_PIN, 0);

	// Wait to make enable wide
	_delay_us(10);

	// FORCE Direction of PORT to OUTPUT

	DIO_vidSetPortxDir(LCD_PORT_DATA, 1);
}

// Send Data to display
void LCD_vidData(s16 data)
{

	LCD_vidInstruction(data, write_operation, DR);
}

// Display string from user
char LCD_String(char *ch)
{
	u8 i;

	// Iterate over string characters
	for(i = 0; ch[i] != '\0'; i++)
	{
		LCD_vidCheckMyLocationAndMove();
		LCD_vidData(ch[i]);
	}
	_delay_ms(1000);

	// Return length of string
	return i;
}


// Define a location to write to
void LCD_vidGoTo(u8 x, u8 y)
{
	// Array with 2 possible lines
	u8 location[] = {0x00, 0x40};

	// Configuring the instruction for the location acquired
	u8 address = location[y-1] + x - 1;

	// Just complete the instruction from data_sheet
	address = address + (1<<7);

	// Execute instruction
	LCD_vidInstruction(address, write_operation, IR);
	_delay_us(20);
}


u8 LCD_u8CurrentLocation_AC(void)
{
	u8 final_location = 0;

	// READ_FROM IR and assign to GLOBAL VAR. DATA
	LCD_vidInstruction(0, read_operation, IR);

	// to lose the value of BF and get only AC value
	final_location = DATA & 0x7f;

	return final_location;
}


// Clear display via start and end locations specified by the user
void LCD_vidClearOnDemand(u8 start, u8 end)
{

	// Start where the user has provided
	LCD_vidGoTo(start, 1);

	// Move with addresses to clear the display pixel by pixel
	for(start; start <= end; start ++)
	{
		// Indication for second line in LCD
		if(start > 16)
		{
			start = 1;
			LCD_vidGoTo(start, 2);
			end -= 16;
		}
		LCD_vidInstruction(0x20, write_operation, DR);
	}

	LCD_vidGoTo(1, 1);

}


void LCD_vidCheckMyLocationAndMove(void)
{
	if(LCD_u8CurrentLocation_AC() == 0x0f)
	{
		LCD_vidGoTo(1,2);
	}
	else if(LCD_u8CurrentLocation_AC() == 0x4f)
	{
		// Display Clear
		LCD_vidInstruction(0x01, write_operation, IR);

		// Delay > 1.53ms
		_delay_ms(5);
		// Return length of string
		LCD_vidGoTo(1,1);
	}
	else;
}

