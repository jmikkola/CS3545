################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/mathlib/mathlib.c 

OBJS += \
./sources/mathlib/mathlib.o 

C_DEPS += \
./sources/mathlib/mathlib.d 


# Each subdirectory must supply rules for building sources it contributes
sources/mathlib/%.o: ../sources/mathlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


