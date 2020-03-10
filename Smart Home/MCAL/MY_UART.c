/*
 * UART.c
 *
 *  Created on: Oct 4, 2019
 *      Author: Newname
 */


#include "../macros.h"
#include "../types.h"
#include "MY_UART.h"
#include <stdlib.h>
#include "DIO.h"
#include "../EMEMBERS.h"
#include "../eeprom.h"
#include "../HAL/LCD.h"


// UCSRA bits
#define RXC		7
#define TXC		6
#define UDRE	5
#define FE		4
#define DOR		3
#define PE		2
#define U2X		1
#define MPCM	0


// UCSRB bits
#define RXCIE	7
#define TXCIE	6
#define RXEN	4		// ENABLE TRANSMITTER
#define TXEN	3		// ENABLE RECIEVER
#define UCSZ2	2		// CHARACTER SIZE
#define RXB8	1
#define TXB8	0

// UCSRC BITS			// WRITE 1 ,1 ONLY ; the whole reg is 10000110
#define URSEL_UCSRC	7	// Ïíå Çááí ÈÊÞÑÑ ÇäÊ åÊÔÊÛá Úáí Ïå æáÇ Çááí ÊÍÊíå
#define UMSEL	6		//
#define UPM1	5
#define UPM0	4		//EDITED
#define USBS	3
#define UCSZ1	2
#define UCSZ0	1
#define UCP0L	0

// COMMON BIT BETWEEN UBRRH AND UCSRC
#define URSEL_UBRRH		15

unsigned char GENERAL_PURPOSE_REGISTER[15] = {0}, Terminate_deadly_signal = 0, string_termination_flag = 0;

void UART_Init(void)
{
  // Set BaudRate -> 9600/8MhZ
  UBRRL = 0x33;
  UBRRH = 0;
  // Set Frame Format -> 8 data, 1 stop, No Parity
  UCSRC = 0x86;
  // Enable RX and TX
  UCSRB = 0x18;
}

void UART_SendChar(u8 data)
{
  // Wait until transmission Register Empty
  while((UCSRA&0x20) == 0x00);
  UDR = data;
}


void UART_SendString(u8 *string, u8 location)
{
	UART_SendChar('$');
	UART_SendChar(location + 0X30);
	UART_SendChar('<');

	for(u8 i = 0 ; string[i] != '\0' ; i++)
	{
		if((string[i] == '*') ||(string[i] == '.'));
		else
			UART_SendChar(string[i]);
	}

}


u8 UART_GetChar(void)
{
  u8 Result;
  // Wait until Reception Complete
  while((UCSRA&0x80) == 0x00);
  Result = UDR;

  UART_CheckValidCharInput(Result);

  Result = EMEMBERS_ALL_LOWER(Result);

  /* Clear Flag */
  set_bit(UCSRA,7);
  return Result;
}


char *UART_GetString(u8 mode, u8 memory_desired)
{
	u8 i = 0, j = 0, f = 0, Number_of_Bytes = 0, x = 0;    // Length counter
	u8 APPEND_FLAG = 0;

	//char *buffer = malloc(15 * sizeof(char)), *buffer_help = NULL;

	if(mode == NAME_CHOISE)
		Number_of_Bytes = 9;
	else if (mode == PASSWORD_CHOISE)
		Number_of_Bytes = 6;
	else
		Number_of_Bytes = 0;

	// Don't append 2 *; before and after. Just return the string as it's
	if(memory_desired == EEPROM_MEMORY);

	// Append 2 *; before and after as it's needed to be compared with each string in memory
	else
		APPEND_FLAG = 1;


	if(APPEND_FLAG)
	{
		GENERAL_PURPOSE_REGISTER[i] = '*';
		i++;
	}

	// Accumulate the string
	for(j = 0; (j < Number_of_Bytes) && (f != 5); j++)
	{

		x = UART_GetChar();

		// Check for valid operation with valid input
		if(Terminate_deadly_signal)
		{
			string_termination_flag = 1;
			// invalid data and Termination flag is up. Return NULL address incase of a string
			return NULL;
		}

		// valid data keep going
		else;

		// Exit condition for names/pass less than the required length
		if(x == '+')
		{
			// terminate the loop and decide later whether you need padding or not
			f = 5;
		}
		// Append char read into array for further operations
		else
		{
			GENERAL_PURPOSE_REGISTER[i] = x;
			// Show characters inserted into array
			LCD_vidData(GENERAL_PURPOSE_REGISTER[i]);
			i++;
		}
	}

	// Valid only for short names/passwords.
	if(f == 5)
		j--;

	// Padding with '.' to fullfill the entire NoOfBytes specified.
	while(j < Number_of_Bytes)
	{
		GENERAL_PURPOSE_REGISTER[i] = '.';
		j++;
		i++;
	}

	if(APPEND_FLAG)
	{
		GENERAL_PURPOSE_REGISTER[i] = '*';
		APPEND_FLAG = 0;
		i++;
	}

	// Terminate with '\0' for further checking on string
	GENERAL_PURPOSE_REGISTER[i] = '\0';

	// Return anything other than NULL for Successful operation
	return 0x01;
}


// Check for '-' char as it's designed to have certain operations
void UART_CheckValidCharInput(u8 result)
{
	if(result == '-')
	{
		// Invalid operations comming; avoid them
		Terminate_deadly_signal = 1;
		//return 0;
	}
	else
	{
		// valid input from user; keep going into operations and
		Terminate_deadly_signal = 0;
	}
}
