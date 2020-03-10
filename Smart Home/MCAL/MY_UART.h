

#ifndef UART_H_
#define UART_H_

extern unsigned char GENERAL_PURPOSE_REGISTER[15], Terminate_deadly_signal, string_termination_flag;


#define EEPROM_MEMORY 	0


#define UDR		*((volatile u8*) 0x2c)
#define	UCSRA	*((volatile u8*) 0x2b)
#define UCSRB	*((volatile u8*) 0x2a)
#define UCSRC	*((volatile u8*) 0x40)

#define	UBRRH	*((volatile u8*) 0x40)
#define UBRRL	*((volatile u8*) 0x29)



void UART_CheckValidCharInput(u8 result);
void UART_Init(void);

void UART_SendChar(u8 data);
void UART_SendString(u8 *string, u8 location);
char *UART_GetString(u8 mode, u8 memory_desired);
u8 UART_GetChar(void);


#endif /* UART_H_ */
