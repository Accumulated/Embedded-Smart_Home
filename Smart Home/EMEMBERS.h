
#ifndef MEMBERS_H_
#define MEMBERS_H_

#include <stdbool.h>


#define REGISTER_ME		0
#define LOGIN_ME		1

extern char global_location_for_memory_and_list_validation;

void LinkedList_vidInitialize(void);
bool LinkedList_vidInsertion(char *name, char location)	;
void LinkedList_vidCompanion(u16 show);
bool LinkedList_show(u8 mode);
char LinkedList_vidSearch(char *ptr, char mode, char process);
char LinkedList_u8DeleteEntire(void);
void LinkedList_vidRestart (void);
void restart_vip(void);
char Authorize_user_login(u8 mode);

bool CompareString(char *str1, char* str2);
char *CopyString(char *ptr1, char *ptr2);

u8 LengthString(char *str1, char *str2);

unsigned char MEMBER_EEPROM_ADMIN_INTERCATION(void);

char EMEMBERS_ALL_LOWER(char x);


#endif
