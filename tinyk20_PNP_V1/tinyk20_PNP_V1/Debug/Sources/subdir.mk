################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Application.c \
../Sources/Events.c \
../Sources/buttons.c \
../Sources/comm.c \
../Sources/encoder.c \
../Sources/main.c \
../Sources/motor.c \
../Sources/platform.c 

OBJS += \
./Sources/Application.o \
./Sources/Events.o \
./Sources/buttons.o \
./Sources/comm.o \
./Sources/encoder.o \
./Sources/main.o \
./Sources/motor.o \
./Sources/platform.o 

C_DEPS += \
./Sources/Application.d \
./Sources/Events.d \
./Sources/buttons.d \
./Sources/comm.d \
./Sources/encoder.d \
./Sources/main.d \
./Sources/motor.d \
./Sources/platform.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/Users/Simon/Documents/HSLU/FS18/INTRO/Workspace/tinyk20_PNP_V1/Static_Code/PDD" -I"C:/Users/Simon/Documents/HSLU/FS18/INTRO/Workspace/tinyk20_PNP_V1/Static_Code/IO_Map" -I"C:/Users/Simon/Documents/HSLU/FS18/INTRO/Workspace/tinyk20_PNP_V1/Sources" -I"C:/Users/Simon/Documents/HSLU/FS18/INTRO/Workspace/tinyk20_PNP_V1/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


