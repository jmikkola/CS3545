################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/models/renderer_model_ASE.c 

OBJS += \
./sources/models/renderer_model_ASE.o 

C_DEPS += \
./sources/models/renderer_model_ASE.d 


# Each subdirectory must supply rules for building sources it contributes
sources/models/%.o: ../sources/models/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


