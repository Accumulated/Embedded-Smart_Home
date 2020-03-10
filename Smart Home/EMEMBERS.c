

#include "HAL/LCD.h"
#include "MCAL/DIO.h"

#include "EMEMBERS.h"
#include "EBOOKS.h"

#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include "eeprom.h"

#include "MCAL/MY_UART.h"

#define AVAILABLE_SHARES 	5

// Define linked list structure
typedef struct members
{
	char *NAME_STRUCT;
	u8 INDEX_STRUCT;
	u8 location_in_memory;
	struct members *NEXT_STRUCT;

}members;

// identify root and a ptr to move from root
members *root_member, *ptr_member;
s8 NoOfMembers = 0, B_insertion_flag = 0;

char global_location_for_memory_and_list_validation = 0;
// Indication for members locations in members_list
u8 INDEX_MEMBERS_GLOBAL = 0;


members *search (char *name, char mode);//, char OLD_BOOKS)



/********************************* READY *********************************/
// Initialize members list
void LinkedList_vidInitialize(void)
{
	// Initializing list
	root_member = malloc(sizeof(members));
	root_member -> NAME_STRUCT = NULL;
	root_member -> NEXT_STRUCT	= NULL;
	//root_member -> ID_STRUCT = NULL;
	root_member -> INDEX_STRUCT = 0;
	root_member -> location_in_memory = 0;
	
	ptr_member = root_member;
}


/********************************* READY *********************************/
// Insertion for values from user
bool LinkedList_vidInsertion(char *name, char location)			// if still, you can change to u8
{
	int k = 0, i = 0;
	char tmp11 = 0;
	
	// Inserting sequence initialized
	ptr_member = root_member;
	members *tmp = malloc(sizeof(members));
	
	if(!tmp)
	{
		LCD_String("5555555555");
		return false;
	}

	// For future reference
	NoOfMembers++;
	INDEX_MEMBERS_GLOBAL++;

	

	// Get member's name length
	while(name[k] != '\0')
	{
		k++;
	}
	
	tmp -> NAME_STRUCT = malloc(k+1 *sizeof(char));


	// Insert patient details
	tmp -> NEXT_STRUCT = ptr_member -> NEXT_STRUCT;

	tmp -> INDEX_STRUCT = INDEX_MEMBERS_GLOBAL;
	
	tmp -> location_in_memory = location;

	for(i = 0; i < k ; i++)
	{
		tmp11 = EMEMBERS_ALL_LOWER(name[i]);
		tmp ->  NAME_STRUCT[i] = tmp11;
	//	LCD_vidData(tmp -> NAME_STRUCT[i]);
	}
	tmp ->  NAME_STRUCT[i] = '\0';

	//LCD_vidData(tmp -> NAME_STRUCT[i]);


	// Update list
	ptr_member -> NEXT_STRUCT = tmp;

	// Indicate success
	//LCD_String("MEMBER INSERTED");
	//_delay_ms(500);
	//LCD_vidClearOnDemand(1, 32);
	return true;
}



/********************************* READY *********************************/
char LinkedList_u8DeleteEntire(void)
{
	// Buffer to save the previous condition of ptr
	members *last_before_stack = NULL;

	// Checking on the next field
	if(ptr_member -> NEXT_STRUCT != NULL)
	{
		last_before_stack = ptr_member;
		ptr_member = ptr_member -> NEXT_STRUCT;
		LinkedList_u8DeleteEntire();
		ptr_member = last_before_stack;
	}

	// free current node and return
	free(ptr_member);
	NoOfMembers = 0;
	return 0;
}



void LinkedList_vidRestart (void)
{
	restart_vip();
	LinkedList_u8DeleteEntire();
	LinkedList_vidInitialize();
}


void restart_vip(void)
{
	ptr_member = root_member;
}

// Mode = USER/PASS,
char LinkedList_vidSearch(char *ptr, char mode, char process)
{
	members *ptr22 = NULL;
	ptr22 = search(ptr, mode);

	if(process == REGISTER_ME)
	{
		if(!ptr22)
		{
			// Indication for not exsisting username/pass, a null pointer returns
			return '0';
		}
		else
			// User/pass exsists in memory already
			return '1';
	}
	else
	{
		if(!ptr22)
		{
			// Indication for not exsisting username/pass, a null pointer returns
			return '0';
		}
		else
		{
			// User/pass exsists in memory already
			if(Authorize_user_login(mode) == '1')
				return '1';
			else
				return '0';
		}
	}

	return '0';
}

// To search for a member
members *search (char *name, char mode)//, char OLD_BOOKS)
{
	u8 name_flag = 0;
	if((mode == NAME_CHOISE) || (mode == ADMIN_CHOISE))
	{
		if((mode == ADMIN_CHOISE))
		{
			EEpromRead_SPECIAL();
		}
		else
		{
			EEpromRead_Enhanced(NAME_CHOISE);
		}
		name_flag = 1;
	//	EEpromRead_Enhanced(NAME_CHOISE);
	}
	else if(mode == PASSWORD_CHOISE)
	{
		EEpromRead_Enhanced(PASSWORD_CHOISE);
	}
	else;

	// Start after the root
	ptr_member = root_member -> NEXT_STRUCT;

	bool found = false;
	
	members *tmp = NULL;
	

	// Iterating over the linked list after root node
	for(int i = 1; i <= NoOfMembers; i++)
	{
		if(CompareString(name, ptr_member -> NAME_STRUCT))
		{
			found = true;
			return  ptr_member;
		}
		ptr_member = ptr_member -> NEXT_STRUCT;
	}

	if(!found)
	{
		//LCD_String("error");
		//_delay_ms(500);

		//LCD_vidClearOnDemand(1, 32);
		ptr_member = root_member;
		return NULL;
	}

	ptr_member = root_member;
	return tmp;
}


// Show all members
bool LinkedList_show(u8 mode)
{
	ptr_member = root_member -> NEXT_STRUCT;

	LCD_vidGoTo(1, 1);
	LCD_String("NoOfMembers are:");
	LCD_vidGoTo(1, 2);
	LinkedList_vidCompanion(NoOfMembers);
	_delay_ms(1000);
	LCD_vidClearOnDemand(1, 32);


	// Only root is no valid list
	if(!ptr_member)
	{
		LCD_String("NO Members");
		_delay_ms(2000);
		LCD_vidClearOnDemand(1, 32);
		ptr_member = root_member;
		return false;
	}


	do
	{
		LCD_vidGoTo(1, 1);
		LCD_String("MEMB_NAME:");
		LCD_String(ptr_member -> NAME_STRUCT);

		_delay_ms(1000);
		LCD_vidClearOnDemand(1, 32);
		if(mode == NAME_CHOISE)
			UART_SendString(ptr_member -> NAME_STRUCT, ptr_member ->location_in_memory);

		ptr_member = ptr_member -> NEXT_STRUCT;
	}
	while(ptr_member != NULL);
	
	ptr_member = root_member;

	return true;
}


//  for user screen output
void LinkedList_vidCompanion(u16 show)
{

	u16 *ptr = malloc(5 * sizeof(u16));
	s8 k = 0, i = 0 ;

	// get values in order
	do
	{
		ptr[i] = (show%10) + 0x30;
		i++;
		show /= 10;
	}while(show);

	ptr[i] = '\0';

	// get string length
	for(k = 0; ptr[k] != '\0'; k++);

	//LCD_vidGoTo(1,2);

	// reverse the string sent
	for(s8 kk = k-1; kk >= 0; kk-- )
	{
		LCD_vidData(ptr[kk]);
	}


	free(ptr);
}

bool CompareString(char *str1, char* str2)
{
	u8 equal = 0;
	equal = LengthString(str1, str2);
	if(equal)
	{
		for(u8 i = 0; str1[i] != '\0'; i++)
		{
			if(str1[i] != str2[i])
				return false;
		}
		return true;
	}
	return false;

}



u8 LengthString(char *str1, char *str2)
{
	u8 i = 0, k = 0;
	while(str1[i] != '\0')
	{
		i++;
	}
	while(str2[k] != '\0')
	{
		k++;
	}

	if(k == i)
	{
		return true;
	}
	return false;

}


// check for user data for any cases that might happen
char Authorize_user_login(u8 mode)
{

	if((mode == NAME_CHOISE))
	{
		// Correct username
		global_location_for_memory_and_list_validation = ptr_member -> location_in_memory;
		return '1';
	}

	// incase of admin choise don't check for password location; no relation between them
	else if(mode == ADMIN_CHOISE)
	{
		// admin is always right
		return '1';
	}

	else
	{
		// check for user index if password is correct
		if(global_location_for_memory_and_list_validation == ptr_member -> location_in_memory)
		{
			// Correct password
			global_location_for_memory_and_list_validation = 0;
			return '1';
		}
		else
		{
			global_location_for_memory_and_list_validation = 0;
			// Incorrect password
			return '0';
		}
	}

	return '0';
}


// Copy string
char *CopyString(char *ptr1, char *ptr2)
{
	u8 j = 0, i = 0;

	// Get Name
	for(j = 1; (ptr1[j] != '\0') && (ptr1[j] != '*'); j++)
	{
		ptr2[i] = ptr1[j];
		i++;
	}
	ptr2[i] = '\0';

	return ptr2;
}


unsigned char MEMBER_EEPROM_ADMIN_INTERCATION(void)
{
	u8 tmp1 = 0, *ptr = NULL;

	EEpromRead_Enhanced(NAME_CHOISE);

	ptr_member = root_member -> NEXT_STRUCT;

	if(!ptr_member)
	{
		UART_SendChar('"');
		ptr_member = root_member;
		return 0;
	}

	ptr = EEPROM_Send_Me_The_address_please(PASSWORD_CHOISE);

//	EEpromRead_Enhanced(NAME_CHOISE);

	LinkedList_show(NAME_CHOISE);


	// Wait for the string to delete from memory

	LCD_String("enter the index to remove");
	LCD_vidClearOnDemand(1, 32);

	UART_SendChar('&');

	// wait for the responce of index
	tmp1 = UART_GetChar() % 0x30;

	LinkedList_vidCompanion(tmp1);

	//if(tmp1 >= 0x30 && tmp1 <= 0x39)
		// Block numbers greater than 9 in the app **IMPORTANT
		// Put a zero in password location and let the function do the rest
	for(u8 j =0; j < 8; j++)
	{
		EEpromWriteByte(ptr[tmp1] + j, '0');
	}
	//EEpromWriteByte(ptr[tmp1], '0');

	EEPROM_ValidateMemoryUserPass();

	ptr_member = root_member;
	return 1;
}


char EMEMBERS_ALL_LOWER(char x)
{
	if(x >= 'A' && x <= 'Z')
		return x + 32;
	else
		return x ;
}
