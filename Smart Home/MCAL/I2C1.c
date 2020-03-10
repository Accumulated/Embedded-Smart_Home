/*
 * I2C1.c
 *
 *  Created on: Jan 26, 2020
 *      Author: Newname
 */

#include "I2C1.h"
#include <util/delay.h>
#include "../HAL/LCD.h"
#include "../EMEMBERS.h"

void IIC_vidSetBitRatePrescaler(u8 prescale)
{
	switch(prescale)
	{
		case 1:
			clr_bit(TWSR, TWPS0);
			clr_bit(TWSR, TWPS1);
			break;
		case 4:
			set_bit(TWSR, TWPS0);
			clr_bit(TWSR, TWPS1);
			break;
		case 16:
			set_bit(TWSR, TWPS1);
			clr_bit(TWSR, TWPS0);
			break;
		case 64:
			set_bit(TWSR, TWPS0);
			set_bit(TWSR, TWPS1);
			break;
		default:
			clr_bit(TWSR, TWPS0);
			clr_bit(TWSR, TWPS1);
			break;
	}
}


						/* AVR TWI Master Mode operation*/

/* Initialization */
void IIC_vidMasetrInitialization(void)
{
	// No prescale is set
    TWSR=0;
    // Bitrate value
    TWBR = 0x10;
    // Enable TWI module
    TWCR|=(1<<TWEN);
}

/* Transmit START condition */
void IIC_vidGeneralStart(u8 mode)
{
	_delay_ms(5);

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);

	// 2 Modes of operation for start signal
	switch(mode)
	{
		case NORMAL_START:

			// Check for the acknowledgment
			while((TWSR & 0xF8) != START_ACK);
			break;

		case REPEATED_START:
			// Check for the acknowledgment
			while((TWSR & 0xF8) != REP_START_ACK);
			break;

		default:
			break;
	}
}

/* Send data or address */
void IIC_vidMasetrWrite(u8 choise, u8 value)
{
	u8 z = 0;
	_delay_ms(5);
	// NOTE YOU EITHER WRITE DATA OR ADDRESS on bus
	switch(choise)
	{
		case DATA_IIC:
			TWDR = value ;
			TWCR = (1 << TWINT) | (1 << TWEN);

			// Wait for the transfer to complete
			while ((TWCR & (1 << TWINT)) == 0);

			// Check for the acknowledgment
			while((TWSR & 0xF8) != WR_BYTE_ACK);
			break;

		case ADDRESS_IIC:
			TWDR = value ;
			TWCR = (1 << TWINT) | (1 << TWEN);

			// Wait for the transfer to complete
			while ((TWCR & (1 << TWINT)) == 0);
			 z = (TWSR & 0xF8);
			 // Check for the More than address related acknowledgments
			while(z != SLAVE_ADD_AND_WR_ACK)
			{
				if(z == WR_BYTE_ACK)
					z = SLAVE_ADD_AND_WR_ACK;
				if(z == SLAVE_ADD_AND_RD_ACK)
					z = SLAVE_ADD_AND_WR_ACK;
			}
			break;

		default:
			break;
	}
	// ADDRESS GRANTED or W_DATA GRANTED
}

/* Receive data */
u8 IIC_vidMasetrRead(u8 mode)
{
	// Incase of Signle byte read
	if(mode == ONE_BYTE)
	{
		TWCR = (1 << TWINT) | (1 << TWEN);
		// Wait for data transfer
		while ((TWCR & (1 << TWINT)) == 0);

		 // Check for the acknowledgment
		while((TWSR & 0xF8) != RD_BYTE_WITH_NACK);
	}

	// Incase of Multibute burst read
	else
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		// Wait for data transfer
		while ((TWCR & (1 << TWINT)) == 0);

		// Check for the acknowledgment
		while((TWSR & 0xF8) != 0x50);
	}

	// DATA GRANTED
	return TWDR;
}

/*  Transmit STOP condition */
void IIC_vidGeneralStop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

