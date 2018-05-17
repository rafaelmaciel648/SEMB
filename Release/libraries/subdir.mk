################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/scheduler.c \
../libraries/timer_tools.c 

OBJS += \
./libraries/scheduler.o \
./libraries/timer_tools.o 

C_DEPS += \
./libraries/scheduler.d \
./libraries/timer_tools.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/%.o: ../libraries/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\rafae\OneDrive\Documentos\WorkspaceEclipseC\SEMB\libraries" -Wall -Os -ffunction-sections -fdata-sections -std=gnu99 -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


