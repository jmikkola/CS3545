################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/SDL_Test.c 

OBJS += \
./sources/SDL_Test.o 

C_DEPS += \
./sources/SDL_Test.d 


# Each subdirectory must supply rules for building sources it contributes
sources/%.o: ../sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

