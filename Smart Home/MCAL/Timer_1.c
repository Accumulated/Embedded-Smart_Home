/*
 * Timer.c
 *
 *  Created on: ??˛/??˛/????
 *      Author: MooDy
 */



#include "../types.h"
#include "DIO.h"
#include "../macros.h"
#include "Timer_1.h"
#define TIMSK_REG *((volatile u8*)0x59)
#define TCCR1A_REG *((volatile u8*)0x4F)
#define TCCR1B_REG *((volatile u8*)0x4E)
#define TCNT1H_REG *((volatile u8*)0x4D)
#define TCNT1L_REG *((volatile u8*)0x4C)
#define OCR1AH_REG *((volatile u8*)0x4B)
#define OCR1AL_REG *((volatile u16*)0x4A)
#define OCR1BH_REG *((volatile u8*)0x49)
#define OCR1BL_REG *((volatile u16*)0x48)
#define SFIOR_REG *((volatile u8*)0x50)
#define ICR1L_REG *((volatile u16*)0x46)
#define ICR1H_REG *((volatile u8*)0x47)
#define TIFR_REG *((volatile u8*)0x58)


extern void Timer1_Vid_Set_CLK_Prescaler_TCCR1B_Reg (u8 clk)
{
	// Ì›÷· œÂ Ì»ﬁÏ «Œ— Õ«Ã… ⁄·‘«‰ «· «Ì„— „Ì⁄œ‘ ›Ï «·«Ê·
	switch(clk)
	{
		case No_CLK:
		{
			clr_bit(TCCR1B_REG,0);
			clr_bit(TCCR1B_REG,1);
			clr_bit(TCCR1B_REG,2);
		}break;
		case CLK:
		{
			set_bit(TCCR1B_REG,0);
			clr_bit(TCCR1B_REG,1);
			clr_bit(TCCR1B_REG,2);
		}break;
		case CLK8:
		{
			clr_bit(TCCR1B_REG,0);
			set_bit(TCCR1B_REG,1);
			clr_bit(TCCR1B_REG,2);
		}break;
		case CLK64:
		{
			set_bit(TCCR1B_REG,0);
			set_bit(TCCR1B_REG,1);
			clr_bit(TCCR1B_REG,2);
		}break;
		case CLK256:
		{
			clr_bit(TCCR1B_REG,0);
			clr_bit(TCCR1B_REG,1);
			set_bit(TCCR1B_REG,2);
		}break;
		case CLK1024:
		{
			set_bit(TCCR1B_REG,0);
			clr_bit(TCCR1B_REG,1);
			set_bit(TCCR1B_REG,2);
		}break;
	}
}



extern void Timer1_Vid_Set_TimerCounter_OCR1AL_Reg(u16 Value)
{
	OCR1AL_REG=Value;
}


extern void Timer1_Vid_Set_Timer_INT_Enable_TIMSK_Reg(void)
{
	set_bit(TIMSK_REG,4);
}

extern void Timer1_Vid_Set_CTC_TCCR1B_Reg(void)
{
	set_bit(TCCR1B_REG,3);
}

void(*Timer_CallBack)(void)=0;

ISR(7)
{
	if(Timer_CallBack!=0)
	{
		Timer_CallBack();
	}
}

extern void Timer_SetCallBack(void(*Pfn)(void))
{
	Timer_CallBack=Pfn;
}

extern void Timer1_Vid_Set_ICR1L_Reg(u16 Value)
{
	ICR1L_REG=Value;
}

extern void Timer1_Vid_Fast_PWM_INT(void)
{
	set_bit(TCCR1A_REG,1);
	set_bit(TCCR1A_REG,5);
	// Enable FAST PWM
	// WGM11
	set_bit(TCCR1A_REG,1);
	// WGM12
	set_bit(TCCR1B_REG,3);
	// WGM13
	set_bit(TCCR1B_REG,4);
	// Bin 7 COM1A1
	set_bit(TCCR1A_REG,7);

}
