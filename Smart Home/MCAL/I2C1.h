/*
 * I2C1.h
 *
 *  Created on: Jan 26, 2020
 *      Author: Newname
 */

#ifndef I2C1_H_
#define I2C1_H_

#include "../types.h"
#include "../macros.h"
#include "DIO.h"


#define F_CPU 8000000ul

// Two-wire Serial Interface Data Register
#define TWDR		*((volatile u8*)0x23)

// Two-wire Serial Interface Address Register
#define TWAR		*((volatile u8*)0x22)
// TWAR Register bits; bit2 is empty
#define TWA6		7
#define TWA5		6
#define TWA4		5
#define TWA3		4
#define TWA2		3
#define TWA1		2
#define TWA0		1
#define TWGGCE		0

// Two-wire Serial Interface Status Register
#define TWSR		*((volatile u8*)0x21)
// TWSR Register bits; bit2 is empty
#define TWS7		7
#define TWS6		6
#define TWS5		5
#define TWS4		4
#define TWS3		3
#define TWPS0		1
#define TWPS1		0

// Two-wire Serial Interface Address Register
#define TWCR		*((volatile u8*)0x56)
// TWAR Register bits; bit2 is empty
#define TWINT		7
#define TWEA		6
#define TWSTA		5
#define TWSTO		4
#define TWWC		3
#define TWEN		2
#define TWIE		0


// Two-wire Serial Interface Bit Rate Register
#define TWBR		*((volatile u8*)0x20)


#define START_ACK                0x08 // start has been sent
#define REP_START_ACK            0x10 // repeated start
#define SLAVE_ADD_AND_WR_ACK     0x18 // Master transmit ( slave address + Write request ) ACK
#define SLAVE_ADD_AND_RD_ACK     0x40 // Master transmit ( slave address + Read request ) ACK
#define WR_BYTE_ACK              0x28 // Master transmit data ACK
#define RD_BYTE_WITH_NACK        0x58 // Master received data with not ACK
#define SLAVE_ADD_RCVD_RD_REQ    0xA8 // means that slave address is received with write req
#define SLAVE_ADD_RCVD_WR_REQ    0x60 // means that slave address is received with read req
#define SLAVE_DATA_RECEIVED      0x80 // means that read byte req is received
#define SLAVE_BYTE_TRANSMITTED   0xC0 // means that write byte req is received

void IIC_vidSetBitRatePrescaler(u8 prescale);
void IIC_vidMasetrInitialization(void);
void IIC_vidGeneralStart(u8 mode);
void IIC_vidMasetrWrite(u8 choise, u8 value);
u8 IIC_vidMasetrRead(u8 mode);
void IIC_vidGeneralStop(void);


void IIC_vidSlaveInitialization(u8 slave_address);
void IIC_vidSlaveListen(void);
void IIC_vidslaveWrite(u8 value);
u8 IIC_vidSlaveRead(u8 isLast);


// either choose 1 or 2 in write_operation
#define DATA_IIC		1
#define ADDRESS_IIC		2
// either choose 1 or 2 in Start_operation
#define NORMAL_START	1
#define REPEATED_START	2

#define ONE_BYTE 		0
#endif /* I2C1_H_ */
