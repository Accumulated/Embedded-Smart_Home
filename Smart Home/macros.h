#ifndef MACROS_H_
#define MACROS_H_
/*#define set_bit(var, bitNo) 		var |= 1<<bitNo

#define clr_bit(var, bitNo) 		var &= ~(1<< bitNo)

#define toggle_bit(var, bitNo) 		var ^= 1<< bitNo

#define assign_bit(var, bitNo, value) 		(value) == 0 ? ( clr_bit(var, bitNo) ) : (set_bit(var, bitNo) )


#define get_bit(var,bitno)       var &=  (1<<bitno)

#define set_High_nibble(var)       var |=  240

#define set_Low_nibble(var)       var |=  15

#define Clear_High_nibble(var)       var &=  15

#define Clear_Low_nibble(var)       var &=  240*/
#define set_bit(var,bit_no) 	var|= (1<<bit_no)
#define clr_bit(var,bit_no) 	var&= ~(1<<bit_no)
#define togle_bit(var,bit_no) 	var^= (1<<bit_no)
#define assign_bit(var,bit_no,val) (val==1) ? (set_bit(var,bit_no)) : (clr_bit(var,bit_no))

#define get_bit(var,bit_no)  	(var>>bit_no)&0x01
#define set_low_nipple(var)  	var|=15
#define set_high_nipple(var)  	var|= 0xf0


#define ISR(vect)          \
		void __vector_##vect (void)__attribute__((signal)); \
		void __vector_##vect (void)


#endif
