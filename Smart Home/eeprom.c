/*
 * eeprom.c
 *
 *  Created on: Jan 26, 2020
 *      Author: Newname
 */

#include "MCAL/I2C1.h"
#include "HAL/LCD.h"
#include <util/delay.h>
#include "EMEMBERS.h"
#include "eeprom.h"
#include <stdlib.h>
#include "MCAL/MY_UART.h"
#include "MCAL/Timer_1.h"

#define NO_OF_USERS_ARRAY				10

// Change according to user size; in this case its 2kB memory with 8Bytes written and read per page
#define REMAINING_FROM_9_BYTES_user		4

// Indication for current data in this list if it's either usernames or passwords
u8 Global_memory_read_password_flag = 0, Globla_memory_read_usernames_flag = 0;

u8 PASSWORD_CHOISE = 11, NAME_CHOISE = 10, WIPE_WHOLE = 20, ADMIN_CHOISE = 9;

u8 NoOfUSERS = 10, NUMBER_REGISTERED = 0, *GLOBAL_WIPE = "000000000", *GLOBAL_FOR_EEpromRead_Enhanced = ((void *)0);

// start_name_address + A = final_name_address (both start and final addresses counts for the same name)
// start_pass_address + 7 = final_pass_address (both start and final addresses counts for the same pass)


// i.e. 0x00 -> 0x0A, 0x0B -> 0x15, 0x16 -> 0x20, 0x21 -> 0x2B, 0x2C -> 0x36, ......., 0x63 -> 0x6D.
u8 start_address_for_MEMORY[32] = {
								0x00, 0x08, 0x10,
								0x18, 0x20, 0x28,
								0x30, 0x38, 0x40,
								0x48, 0x50, 0x58,
								0x60, 0x68, 0x70,
								0x78, 0x80, 0x88,
								0x90, 0x98, 0xA0,
								0xA8, 0xB0, 0xB8,
								0xC0, 0xC8, 0xD0,
								0xD8, 0xE0, 0xE8,
								0xF0, 0xF8 };


// i.e. 0x00 -> 0x0A, 0x0B -> 0x15, 0x16 -> 0x20, 0x21 -> 0x2B, 0x2C -> 0x36, ......., 0x63 -> 0x6D.
u8 start_address_for_names[NO_OF_USERS_ARRAY] = {
								0x00, 0x0B, 0x16,
								0x21, 0x2C, 0x37,
								0x42, 0x4D, 0x58,
								0x63};


// i.e. 0x70 -> 0x77, 0x78 -> 0x7F, ......, 0xB8 -> 0xBF.
u8 start_address_for_passwords[NO_OF_USERS_ARRAY] = {
								0x70, 0x78, 0x80,
								0x88, 0x90, 0x98,
								0xA0, 0xA8, 0xB0,
								0xB8};


u8 start_address_for_control[2] = {0xc8, 0xc9};

// 0xCA -> 0xD4 admin name, 0xD5 -> DF admin password
u8 Admin_address_in_memory[2] = {0xCA, 0xD5};

/* NOTE: admin only has 11 bytes for user and 11 bytes for pass	*/
/* NOTE: remeber that admin has his own 22 bytes, back to back, contiguous chunck of memory	*/

u8 GLOBAL_CHOISE_FOR_WRITING = 0;


void EEpromInit(void)
{
	// Simply start avr as MASTER MODE
	IIC_vidMasetrInitialization();
}

// Writing single Byte
void EEpromWriteByte(u8 address, u8 data)
{
	// Sending start signal
	IIC_vidGeneralStart(NORMAL_START);

	// SLA + W up on TWI bus
	IIC_vidMasetrWrite(ADDRESS_IIC, 0b10100000);

	// Sending address for the location to write at
	IIC_vidMasetrWrite(ADDRESS_IIC, address);

	// Sending data to the required address
	IIC_vidMasetrWrite(DATA_IIC, data);

	// Sending stop signal
	IIC_vidGeneralStop();
}

// Writing a whole string with proper termination
void EEpromWriteString(u8 CHOISE_NUMBER, u8 *data, u8 CHOISE_MODE)
{
	u8 i = 0, j = 0, *ptr = 0, name_flag = 0, pass_flag = 0;

	// Choise mode is used to adjust the function for the required operation values
	if((CHOISE_MODE == NAME_CHOISE) || (CHOISE_MODE == ADMIN_CHOISE))
	{
		// For any admin information type it for 11byte in memory
		if(CHOISE_MODE == ADMIN_CHOISE)
		{
			// start at admin location and decide by using choise number if either a user or pass
			ptr = Admin_address_in_memory;
		}

		// The mode is in normal mode for ptr = start_names
		else
		{
			// For 11 bytes of name, 9 are the actual name
			ptr = start_address_for_names;
		}

		// In all cases if writing admin user/pass info or a regular name info raise the flag
		name_flag = 1;
		GLOBAL_CHOISE_FOR_WRITING = 10;
	}

	else if(CHOISE_MODE == PASSWORD_CHOISE)
	{
		// For 8 byte for password, 6 are the actual password
		ptr = start_address_for_passwords;
		pass_flag = 1;
		GLOBAL_CHOISE_FOR_WRITING = 7;
	}

	else
	{
		ptr = start_address_for_MEMORY;
		GLOBAL_CHOISE_FOR_WRITING = 8;
	}


	// Bound a string with 2 '\0'; before and after
	if((name_flag) || (pass_flag))
	{
		// Append '*' at first location sent
		EEpromWriteByte(ptr[CHOISE_NUMBER], '*');
		i = 1;
	}

	// Keep writing data from string

	while((i < GLOBAL_CHOISE_FOR_WRITING) && (data[j] != '\0'))
	{
		// Write in memory
		EEpromWriteByte(ptr[CHOISE_NUMBER] + i, data[j]);
		j++;
		i++;
	}

	// Incase the string is short, keep writing zeros in the reserved bytes
	while( i < GLOBAL_CHOISE_FOR_WRITING)
	{
		EEpromWriteByte(ptr[CHOISE_NUMBER] + i, '.');
		i++;
	}

	// Terminate with a null character
	if(name_flag || pass_flag)
	{
		EEpromWriteByte(ptr[CHOISE_NUMBER] + i, '*');
	}

	// Confrim for data completion
}


// Read a chucnk of memory locations either names or passwords
unsigned char EEpromRead_Enhanced(u8 CHOISE_MODE)
{
	u8 rec = 0, *ptr = ((void *)0), j = 0, Start_Flag_address = 0;
	u8 START_DOT_INSERTION = 0;

	char *pttr = malloc(15 * sizeof(char));

	if(!pttr)
	{
		LCD_String("a7a");
		return 1;
	}
	// Restart linkedlist to prepare for a new memory read
	LinkedList_vidRestart();


	if(CHOISE_MODE == NAME_CHOISE)
	{
		ptr = start_address_for_names;
		GLOBAL_CHOISE_FOR_WRITING = 9;
		Globla_memory_read_usernames_flag = 1;
		Global_memory_read_password_flag = 0;
	}
	else if(CHOISE_MODE == PASSWORD_CHOISE)
	{
		ptr = start_address_for_passwords;
		GLOBAL_CHOISE_FOR_WRITING = 6;
		Global_memory_read_password_flag = 1;
		Globla_memory_read_usernames_flag = 0;
	}
	else;

	// Iterate over names in order
	for(s8 xx = 0; xx < NoOfUSERS; xx++)
	{
		// Indication for start of every single section in each block in memory
		Start_Flag_address = 1;
		START_DOT_INSERTION = 1;

		// Iterate over elements in each chunck of memory
		for(j = 0; j <= GLOBAL_CHOISE_FOR_WRITING; j++)
		{
			rec = EEpromReadByte(ptr[xx] + j);

			// Indicate empty name and terminate the loop
			if((rec == '0') && (Start_Flag_address == 1))
			{
				//Empty user, skip reading this section
				Start_Flag_address = 0;
				j = 40;
			}

			// '*' needs to be inserted before start of each string in the list
			else if((START_DOT_INSERTION == 1) && (Start_Flag_address == 1))
			{
				START_DOT_INSERTION = 0;
				Start_Flag_address = 0;

				// Accumulate linkedlist
				pttr[j] = ('*');
			}

			// Valid only if loop will be terminated for j+1
			else if(j+1 > GLOBAL_CHOISE_FOR_WRITING)
			{
				pttr[j] = rec;
				pttr[j+1] = ('*');
				pttr[j+2] = ('\0');
				LinkedList_vidInsertion(pttr, xx);
			}
			else
			{
				pttr[j] = rec;
			}
		}
	}

	free(pttr);

	//LCD_String("memory read");

    return 0;
}

// Reading single byte
unsigned char EEpromReadByte(u8 address)
{
	u8 data = 0;
	// Sending start signal
	IIC_vidGeneralStart(NORMAL_START);

	// SLA + W
	IIC_vidMasetrWrite(ADDRESS_IIC, 0b10100000);

	// Send Desired location to read from
	IIC_vidMasetrWrite(ADDRESS_IIC, address);

	// Repeate start to complete the initialization
	IIC_vidGeneralStart(REPEATED_START);

	// SLA + R
	IIC_vidMasetrWrite(ADDRESS_IIC, 0b10100001);

	// Read single byte mode
	data = IIC_vidMasetrRead(ONE_BYTE);

	// Sending stop signal
	IIC_vidGeneralStop();

	// Confrim data repeiption

    return data;
}

void EEPROM_reset(void)
{
	u8 * ptr = NULL;

	// 32 * 8bytes per package = 256B is written in memory all zeros
	for(u8 i = 0; i < 32; i++)
	{
		// Send a whole 8B string full of zeros
		EEpromWriteString(i, GLOBAL_WIPE, WIPE_WHOLE);
	}

	// Reset the status of control section in memory
	ptr = start_address_for_control;

	EEpromWriteByte(ptr[LIGHTSOnOff], '0');
	EEpromWriteByte(ptr[DOOR_LorUL], '0');

	// Remember to start admin by definition
	EEPROM_ADMINOnlyAccessInitialize();

	// Confirm wiping every location in memory
	LCD_String("EEPROM WIPED");
	LCD_vidClearOnDemand(1, 16);
}


// Find empty locations in memory and return them to assign
char EEPROM_Empty_Locations(u8 mode_empty)
{
	u8 * ptr = NULL;

	// Check for required field of operation
	if(mode_empty == NAME_CHOISE)
	{
		ptr = start_address_for_names;
	}
	else if(mode_empty == PASSWORD_CHOISE)
	{
		ptr = start_address_for_passwords;
	}
	else;

	// Indicate memory errors with ptr
	if(ptr == NULL)
	{
		LCD_String("WARNING::error mode of operation");
		LCD_vidClearOnDemand(1, 32);
	}

	// Check for empty location in 10 users count
	for(u8 i = 0; i < NoOfUSERS; i++)
	{
		// a plcase is considered empty when it starts with 0; the default value in memory
		if(EEpromReadByte(ptr[i]) == '0')
		{
			// Return empty place location in memory to assign with a value
			return i;
		}
	}

	// Indicate no locaitons are available
	LCD_String("WARNING::Full list of user names");
	LCD_vidClearOnDemand(1, 32);

	return '0';
}



// Check for usernames without passwords

// this case might happen when user exits the program in the middle of it
void EEPROM_ValidateMemoryUserPass(void)
{
	//LCD_String("checking memory..");
	u8 *ptr1 = NULL, *ptr2 = NULL;

	// Pointer1/2 has the names/passwords array address accordingly
	ptr1 = start_address_for_names;
	ptr2 = start_address_for_passwords;

	// Check for empty password locations in memory
	// Check for empty usernames accross each password in memory

	//EEpromReadByte(ptr1[location]);

	for(u8 i = 0; i < NoOfUSERS; i++)
	{
		// Check for empty password fields only
		if(EEpromReadByte(ptr2[i]) == '0')
		{
			// Make sure the correcponding username is also empty
			if(EEpromReadByte(ptr1[i]) == '0');

			else
			{
				for(u8 j = 0; j < 11; j++)
				{
					EEpromWriteByte(ptr1[i] + j, '0');
				}

				//LCD_String("INVALID user is wiped");
				LCD_vidClearOnDemand(1, 32);
			}
		}
	}
}

/*
// Control either a door or lights
void EEPROM_Things_To_Control(void)
{
	u8 *ptr = NULL, indication = 0, local_flag = 0;

	//DIO_vidSetPinxDir(PORTD, 7, OUTPUT);

	// Pointer starts at specified locations in memory
	ptr = start_address_for_control;

	while(1)
	{

		// exit only when you either logout or exit button is pressed
		if(local_flag)
			break;

		// Control door by toggeling its previous state according to what the mode is
		indication = UART_GetChar();

		// Open door
		switch(indication)
		{
			// Door control to open
			case 'J':
				if(EEpromReadByte(ptr[DOOR_LorUL]) == '1')
				{
					// Door is already open
					UART_SendChar('M');
				}
				else
				{
					// Door closes using servo motor
					EEpromWriteByte(ptr[DOOR_LorUL], '1');
					// start servo motor	(EDIT_ME)
					UART_SendChar('M');
				}
				break;

			// Door control to close
			case 'j':
				if(EEpromReadByte(ptr[DOOR_LorUL]) == '0')
				{
					// Door is already closed
					UART_SendChar('m');
				}
				else
				{
					// Door opens using servo motor
					EEpromWriteByte(ptr[DOOR_LorUL], '0');
					// start servo motor in reverse direction	(EDIT_ME)
					UART_SendChar('m');
				}
				break;

			// Lights ON
			case 'K':
				DIO_vidSetPortBitValue(PORTD, 7,OUTPUT);
				EEpromWriteByte(ptr[LIGHTSOnOff], '1');

				// lights is on
				UART_SendChar('O');

				break;

			case 'k':
				// lights off
				DIO_vidSetPortBitValue(PORTD, 7, INPUT);

				// Update memory status
				EEpromWriteByte(ptr[LIGHTSOnOff], '0');

				// lights is off
				UART_SendChar('o');
				break;

			default:
				local_flag = 1;
				break;
		}
	}
}
*/


// Control either a door or lights
/*void EEPROM_Things_To_Control(void)
{
	u8 *ptr = NULL, indication = 0, local_flag = 0;

	DIO_vidSetPinxDir(PORTD, 7, OUTPUT);

	// Pointer starts at specified locations in memory
	ptr = start_address_for_control;

	while(1)
	{

		// exit only when you either logout or exit button is pressed
		if(local_flag)
			break;

		// Control door by toggeling its previous state according to what the mode is
		indication = UART_GetChar();

		// Open door
		switch(indication)
		{
			// Door control to open
			case 'J':
				if(EEpromReadByte(ptr[DOOR_LorUL]) == '1')
				{
					// Door is already open
					UART_SendChar('M');
				}
				else
				{
					// Door closes using servo motor
					EEpromWriteByte(ptr[DOOR_LorUL], '1');
					// start servo motor	(EDIT_ME)
					UART_SendChar('M');
				}
				break;

			// Door control to close
			case 'j':
				if(EEpromReadByte(ptr[DOOR_LorUL]) == '0')
				{
					// Door is already closed
					UART_SendChar('m');
				}
				else
				{
					// Door opens using servo motor
					EEpromWriteByte(ptr[DOOR_LorUL], '0');
					// start servo motor in reverse direction	(EDIT_ME)
					UART_SendChar('m');
				}
				break;

			// Lights ON
			case 'K':
				if(EEpromReadByte(ptr[LIGHTSOnOff]) == '1')
				{
					// lights is already on
					UART_SendChar('O');
				}
				else
				{
					// lights on
					DIO_vidSetPortBitValue(PORTD, 7,OUTPUT);
					// Update memory status
					EEpromWriteByte(ptr[LIGHTSOnOff], '1');
					UART_SendChar('O');
				}
				break;

			case 'k':
				if(EEpromReadByte(ptr[LIGHTSOnOff]) == '0')
				{
					// lights is already off
					UART_SendChar('o');
				}
				else
				{
					// lights off
					DIO_vidSetPortBitValue(PORTD, 7, INPUT);

					// Update memory status
					EEpromWriteByte(ptr[LIGHTSOnOff], '0');
					UART_SendChar('o');
				}
				break;

			default:
				local_flag = 1;
				break;
		}
	}
}

*/

// Control either a door or lights
void EEPROM_Things_To_Control(void)
{
	u8 *ptr = NULL, indication = 0, local_flag = 0;
	u16 val;

	DIO_vidSetPinxDir(PORTD, 7, OUTPUT);
	// --------------- Servo --------------
	DIO_vidSetPinxDir(PORTD, 5, OUTPUT);
	// For ICR1L   50HZ   period 20ms
	Timer1_Vid_Set_ICR1L_Reg(20000-1);
	// For compare value
	// TCCR1A - INVERT MODE
	Timer1_Vid_Fast_PWM_INT();
	// Prescaler 8
	Timer1_Vid_Set_CLK_Prescaler_TCCR1B_Reg(CLK8);

	//  -----------------------------------

	// Pointer starts at specified locations in memory
	ptr = start_address_for_control;

	while(1)
	{

		// exit only when you either logout or exit button is pressed
		if(local_flag)
			break;

		// Control door by toggeling its previous state according to what the mode is
		indication = UART_GetChar();

		// Open door
		switch(indication)
		{
			// Door control to open
			case 'j':
				if(EEpromReadByte(ptr[DOOR_LorUL]) == '1')
				{
					// Door is already open
					UART_SendChar('|');
				}
				else
				{
					// Door closes using servo motor
					EEpromWriteByte(ptr[DOOR_LorUL], '1');
					// start servo motor	(EDIT_ME)
					for(val=1700 ; val>340 ; val=val-20)
					{
						Timer1_Vid_Set_TimerCounter_OCR1AL_Reg(val);
						 _delay_ms(30);
					}
					UART_SendChar('|');
				}
				break;

			// Door control to close
			case 'r':
				if(EEpromReadByte(ptr[DOOR_LorUL]) == '0')
				{
					// Door is already closed
					UART_SendChar(';');
				}
				else
				{
					// Door opens using servo motor
					EEpromWriteByte(ptr[DOOR_LorUL], '0');
					// start servo motor in reverse direction	(EDIT_ME)
					for(val=340 ; val<1650 ; val=val+20)
					{
						Timer1_Vid_Set_TimerCounter_OCR1AL_Reg(val);
						  _delay_ms(30);
					}
					UART_SendChar(';');
				}
				break;

			// Lights ON
			case 'k':
				if(EEpromReadByte(ptr[LIGHTSOnOff]) == '1')
				{
					// lights is already on
					UART_SendChar(':');
				}
				else
				{
					// lights on
					DIO_vidSetPortBitValue(PORTD, 7,OUTPUT);
					// Update memory status
					EEpromWriteByte(ptr[LIGHTSOnOff], '1');
					UART_SendChar(':');
				}
				break;

			case 'p':
				if(EEpromReadByte(ptr[LIGHTSOnOff]) == '0')
				{
					// lights is already off
					UART_SendChar('?');
				}
				else
				{
					// lights off
					DIO_vidSetPortBitValue(PORTD, 7, INPUT);

					// Update memory status
					EEpromWriteByte(ptr[LIGHTSOnOff], '0');
					UART_SendChar('?');
				}
				break;

			default:
				local_flag = 1;
				break;
		}
	}
}

// Call this function only when resetting the memory
void EEPROM_ADMINOnlyAccessInitialize(void)
{
	// Algorithm: ADMIN_CHOISE is used in both cases of user/pass
	// NOTE: It has to be 22 bytes to be a unique identification for user and password
	// NOTE: It's 22 bytes conserved back2back sequence

	// Start by inserting username in memory in this special location in memory
	EEpromWriteString(0, "ADMIN", ADMIN_CHOISE);

	// continue by inserting password for admin
	EEpromWriteString(1, "ADMIN", ADMIN_CHOISE);
}

// Remember the states oflights controlled by the system
unsigned char EEPROM_REMEBER_ME(void)
{
	return EEpromReadByte(start_address_for_control[LIGHTSOnOff]);

}


void EEpromRead_SPECIAL(void)
{
	u8 rec = 0, *ptr = ((void *)0), j = 0, Start_Flag_address = 0;
	u8 START_DOT_INSERTION = 0;

	char *pttr = malloc(15 * sizeof(char));

	// Restart linkedlist to prepare for a new memory read
	LinkedList_vidRestart();

	ptr = Admin_address_in_memory;
	GLOBAL_CHOISE_FOR_WRITING = 9;
	Globla_memory_read_usernames_flag = 1;

	// Iterate over names in order
	for(s8 xx = 0; xx < 2; xx++)
	{
		// Indication for start of every single section in each block in memory
		Start_Flag_address = 1;
		START_DOT_INSERTION = 1;

		// Iterate over elements in each chunck of memory
		for(j = 0; j <= GLOBAL_CHOISE_FOR_WRITING; j++)
		{
			rec = EEpromReadByte(ptr[xx] + j);

			// Indicate empty name and terminate the loop
			if((rec == '0') && (Start_Flag_address == 1))
			{
				//Empty user, skip reading this section
				Start_Flag_address = 0;
				j = 40;
			}

			// '*' needs to be inserted before start of each string in the list
			else if((START_DOT_INSERTION == 1) && (Start_Flag_address == 1))
			{
				START_DOT_INSERTION = 0;
				Start_Flag_address = 0;

				// Accumulate linkedlist
				pttr[j] = ('*');
			}

			// Valid only if loop will be terminated for j+1
			else if(j+1 > GLOBAL_CHOISE_FOR_WRITING)
			{
				pttr[j] = rec;
				pttr[j+1] = ('*');
				pttr[j+2] = ('\0');
				LinkedList_vidInsertion(pttr, xx);
			}
			else
			{
				pttr[j] = rec;
			}
		}
	}

	free(pttr);
	//LinkedList_show();
}


u8 *EEPROM_Send_Me_The_address_please(u8 mode)
{
	if(mode == NAME_CHOISE)
		return start_address_for_names;

	// default for a password
		return start_address_for_passwords;
}
