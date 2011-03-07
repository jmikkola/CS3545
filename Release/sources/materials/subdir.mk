################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/materials/renderer_materials.c 

OBJS += \
./sources/materials/renderer_materials.o 

C_DEPS += \
./sources/materials/renderer_materials.d 


# Each subdirectory must supply rules for building sources it contributes
sources/materials/%.o: ../sources/materials/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


