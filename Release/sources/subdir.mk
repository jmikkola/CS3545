################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/camera.c \
../sources/common.c \
../sources/input.c \
../sources/main.c \
../sources/render.c \
../sources/system_files.c \
../sources/timestep.c 

OBJS += \
./sources/camera.o \
./sources/common.o \
./sources/input.o \
./sources/main.o \
./sources/render.o \
./sources/system_files.o \
./sources/timestep.o 

C_DEPS += \
./sources/camera.d \
./sources/common.d \
./sources/input.d \
./sources/main.d \
./sources/render.d \
./sources/system_files.d \
./sources/timestep.d 


# Each subdirectory must supply rules for building sources it contributes
sources/%.o: ../sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


