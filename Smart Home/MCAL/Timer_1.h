/*
 * Timer_1.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: MooDy
 */

#ifndef MCAL_TIMER_1_H_
#define MCAL_TIMER_1_H_

extern void Timer1_Vid_Set_CLK_Prescaler_TCCR1B_Reg(u8 clk);
extern void Timer1_Vid_Set_TimerCounter_OCR1AL_Reg(u16 Value);
extern void Timer1_Vid_Set_Timer_INT_Enable_TIMSK_Reg(void);
extern void Timer1_Vid_Set_CTC_TCCR1B_Reg(void);
extern void Timer_SetCallBack(void(*Pfn)(void));
extern void Timer1_Vid_Set_ICR1L_Reg(u16 Value);
extern void Timer1_Vid_Fast_PWM_INT(void);
#define No_CLK 0
#define CLK 1
#define CLK8 2
#define CLK64 3
#define CLK256 4
#define CLK1024 5

#endif /* MCAL_TIMER_1_H_ */
