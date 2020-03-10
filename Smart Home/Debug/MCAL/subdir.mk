################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/DIO.c \
../MCAL/I2C1.c \
../MCAL/MY_UART.c \
../MCAL/Timer_1.c 

OBJS += \
./MCAL/DIO.o \
./MCAL/I2C1.o \
./MCAL/MY_UART.o \
./MCAL/Timer_1.o 

C_DEPS += \
./MCAL/DIO.d \
./MCAL/I2C1.d \
./MCAL/MY_UART.d \
./MCAL/Timer_1.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


