/*
 * app.c
 *
 *  Created on: Sep 16, 2019
 *      Author: Newname
 */

#include <stdlib.h>
#include "MCAL/DIO.h"
#include "HAL/LCD.h"
#include "macros.h"
#include "types.h"
#include "EMEMBERS.h"
#include <util/delay.h>
#include "eeprom.h"
#include "MCAL/MY_UART.h"

void DISPLAY_OnDemand(char *str1, char *str2);


int main(void)
{
	// Array to accumulate the name only or password only with just a '\0' as a termination
	u8 array[15] = {0};

	u8 tmp1 = 0, tmp2 = 0, flag_right_password = 0;

	// Initialize all at once
	LCD_vidInitialize();
	LinkedList_vidInitialize();
	EEpromInit();
	//EEPROM_reset();
	UART_Init();

	// PortD is used to control led
	DIO_vidSetPinxDir(PORTD, 7, OUTPUT);

	// Read the status of led either it was on or off
	tmp1 = EEPROM_REMEBER_ME() % 0x30;

	// Remember the last value in memory
	DIO_vidSetPortBitValue(PORTD, 7, tmp1);

	//EEpromRead_SPECIAL();

	while(1)
	{

		EEPROM_ValidateMemoryUserPass();

		if(string_termination_flag)
		{
			string_termination_flag = 0;
		}
		else
		{
			// Wait for the start signal
			tmp1 = UART_GetChar();
		}

		// Incase you started with anything rather than '-' char as it's supposed
		if(!Terminate_deadly_signal);

		else
		{
			Terminate_deadly_signal = 0;

			while(1)
			{
				LCD_String("OPPERATIONs:");

				DISPLAY_OnDemand("1- REGISTER", "2- lOGIN");

				LCD_String("Choise:");
				LCD_vidGoTo(1, 2);


				tmp1 = UART_GetChar();

				// Invalid input char; break the loop
				if(Terminate_deadly_signal)
				{
					string_termination_flag = 1;
					break;
				}

				// Check for empty locations before deciding any operation
				tmp2 = EEPROM_Empty_Locations(NAME_CHOISE);

				// Incase of registration
				if((tmp1 == 'g') && (tmp2 != '0'))
				{

					LCD_vidData(0x31);
					_delay_ms(500);
					LCD_vidClearOnDemand(1, 32);

					LCD_String("Enter user name:");
					LCD_vidGoTo(1, 2);

					// Send start signal to retrieve username
					UART_SendChar('~');

					if(UART_GetString(NAME_CHOISE, 1) == NULL)
					{
						_delay_ms(500);
						LCD_vidClearOnDemand(1, 32);
						// Invalid data and break the loop
						LCD_String("invalid input");
						break;
					}

					// valid data keep going
					else;

					_delay_ms(500);
					LCD_vidClearOnDemand(1, 32);

					CopyString(GENERAL_PURPOSE_REGISTER, array);

					// Check for either a name exsists or number of users exceeded
					if((LinkedList_vidSearch(GENERAL_PURPOSE_REGISTER, NAME_CHOISE, REGISTER_ME) == '1'))
					{
						LCD_String("name error");
						LCD_vidClearOnDemand(1, 32);

						// Remove bytes from the list
						LinkedList_vidRestart();

						UART_SendChar('!');
						break;
					}

					// Only if a user is a valid username
					EEpromWriteString(tmp2, array, NAME_CHOISE);

					// Remove and get ready for another sequence of bytes
					LinkedList_vidRestart();

					LCD_String("Enter password:");
					LCD_vidGoTo(1, 2);

					// Username is saved indication
					UART_SendChar('@');

					// Get the password
					if(!UART_GetString(PASSWORD_CHOISE, 1))
					{
						_delay_ms(500);
						LCD_vidClearOnDemand(1, 32);

						// Invalid data and break the loop
						break;
					}
					// valid data keep going
					else;

					_delay_ms(500);
					LCD_vidClearOnDemand(1, 32);

					CopyString(GENERAL_PURPOSE_REGISTER, array);

					// Insert the password, same location in the array as the user
					EEpromWriteString(tmp2, array, PASSWORD_CHOISE);

					LCD_String("MEMBER ADDED");
					LCD_vidClearOnDemand(1, 16);

					UART_SendChar('#');
					break;
				}

				// Incase of logging in with an exsisting user or password
				else if(tmp1 == 'c')
				{
					LCD_vidData(0x32);
					_delay_ms(500);
					LCD_vidClearOnDemand(1, 32);

					LCD_String("USERNAME:");
					LCD_vidGoTo(1, 2);

					// Send start signal to retrieve information
					UART_SendChar('%');

					if(!UART_GetString(NAME_CHOISE, 1))
					{
						_delay_ms(500);
						LCD_vidClearOnDemand(1, 32);
						// Invalid data and break the loop
						break;
					}
					// valid data keep going
					else;

					_delay_ms(500);
					LCD_vidClearOnDemand(1, 32);

					/* Starting to validate user input sequence	*/

					// First sub-login - Normal user

					// Check for normal user exsistence in memory
					if((LinkedList_vidSearch(GENERAL_PURPOSE_REGISTER, NAME_CHOISE, LOGIN_ME) == '1'))
					{
						LinkedList_vidRestart();

						LCD_String("Enter password:");
						UART_SendChar('^');

						for(u8 iii = 0; iii < 3; iii++)
						{
							LCD_vidGoTo(1, 2);
							// 3 trials are the limit
							if(!UART_GetString(PASSWORD_CHOISE, 1))
							{
								_delay_ms(500);
								LCD_vidClearOnDemand(1, 32);
								// Invalid data and break the loop
								break;
							}
							// valid data keep going
							else;

							_delay_ms(500);
							LCD_vidClearOnDemand(1, 32);

							if((LinkedList_vidSearch(GENERAL_PURPOSE_REGISTER, PASSWORD_CHOISE, LOGIN_ME) == '1'))
							{

								LCD_vidGoTo(1, 1);
								LCD_String("welcome..");
								_delay_ms(500);
								LCD_vidClearOnDemand(1, 32);

								UART_SendChar('(');
								EEPROM_Things_To_Control();
								LinkedList_vidRestart();
								flag_right_password = 1;
								break;
							}

							LCD_String("try again:");

							UART_SendChar(')');
						}
						if(flag_right_password == 1)
						{
							flag_right_password = 0;
							break;
						}
						else
						{
							UART_SendChar('_');
							break;
						}
					}

					/* START IAM MODIFIED REMOVE ME	*/

					// Second case - Admin user
					else if(LinkedList_vidSearch(GENERAL_PURPOSE_REGISTER, ADMIN_CHOISE, LOGIN_ME) == '1')
					{
						LinkedList_vidRestart();

						LCD_String("root password:");
						UART_SendChar('^');

						for(u8 iii = 0; iii < 3; iii++)
						{
							LCD_vidGoTo(1, 2);
							// 3 trials are the limit
							if(!UART_GetString(NAME_CHOISE, 1))
							{
								_delay_ms(500);
								LCD_vidClearOnDemand(1, 32);
								// Invalid data and break the loop
								break;
							}
							// valid data keep going
							else;

							_delay_ms(500);
							LCD_vidClearOnDemand(1, 32);

							if((LinkedList_vidSearch(GENERAL_PURPOSE_REGISTER, ADMIN_CHOISE, LOGIN_ME) == '1'))
							{

								LCD_vidGoTo(1, 1);
								LCD_String("super root granted");
								_delay_ms(500);
								LCD_vidClearOnDemand(1, 32);

								UART_SendChar('=');

								/* Modify me to go to names page and automatically send strings so first you have to read the memory	*/
								MEMBER_EEPROM_ADMIN_INTERCATION();
								LinkedList_vidRestart();
								flag_right_password = 1;
								break;
							}

							LCD_String("try again:");

							UART_SendChar(')');
						}
						if(flag_right_password == 1)
						{
							flag_right_password = 0;
							break;
						}
						else
						{
							UART_SendChar('_');
							break;
						}
					}
					/* END IAM MODIFIED REMOVE ME	*/

					// Incase of an invalid login user in memory
					else
					{
						UART_SendChar('/');
						break;
					}
				}

				// Incase of users exceeds when registration occurs
				else
				{
					UART_SendChar('!');
					break;
				}
			}
		}
	}

	return 0 ;
}

void DISPLAY_OnDemand(char *str1, char *str2)
{
	LCD_vidClearOnDemand(1, 32);
	// Display 1st string
	LCD_vidGoTo(1, 1);
	LCD_String(str1);
	// Display 2nd string
	LCD_vidGoTo(1, 2);
	LCD_String(str2);

	//_delay_ms(2300);
	// Clear whole display
	LCD_vidClearOnDemand(1, 32);
}

