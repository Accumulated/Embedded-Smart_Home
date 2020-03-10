/*
 * eeprom.h
 *
 *  Created on: Jan 26, 2020
 *      Author: Newname
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdbool.h>

extern unsigned char PASSWORD_CHOISE, NAME_CHOISE, WIPE_WHOLE, ADMIN_CHOISE;
extern u8 NoOfalphas, Global_memory_read_password_flag, Globla_memory_read_usernames_flag;


// 1 means door/lights are open/on
// 0 means door/lights are closed/off

#define DOOR_LorUL		0
#define LIGHTSOnOff		1

void EEpromInit(void);

void EEpromWriteByte(u8 address, u8 data);
void EEpromWriteString(u8 CHOISE_NUMBER, u8 *data, u8 CHOISE_MODE);

unsigned char EEpromReadByte(u8 address);
unsigned char EEpromRead_Enhanced(u8 CHOISE_MODE);

char EEPROM_Empty_Locations(u8 mode_empty);

void EEPROM_reset(void);

void EEPROM_ValidateMemoryUserPass(void);

void EEPROM_Things_To_Control(void);

void EEPROM_ADMINOnlyAccessInitialize(void);

unsigned char EEPROM_REMEBER_ME(void);

void EEpromRead_SPECIAL(void);

u8 *EEPROM_Send_Me_The_address_please(u8 mode);




#endif /* EEPROM_H_ */
