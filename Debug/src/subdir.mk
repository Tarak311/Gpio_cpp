################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Tarak0.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc175x_6x.cpp \
../src/gpiopin.cpp 

C_SRCS += \
../src/FreeRTOSCommonHooks.c \
../src/crp.c \
../src/heap_3.c \
../src/list.c \
../src/port.c \
../src/queue.c \
../src/sysinit.c \
../src/tasks.c 

OBJS += \
./src/FreeRTOSCommonHooks.o \
./src/Tarak0.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/gpiopin.o \
./src/heap_3.o \
./src/list.o \
./src/port.o \
./src/queue.o \
./src/sysinit.o \
./src/tasks.o 

CPP_DEPS += \
./src/Tarak0.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc175x_6x.d \
./src/gpiopin.d 

C_DEPS += \
./src/FreeRTOSCommonHooks.d \
./src/crp.d \
./src/heap_3.d \
./src/list.d \
./src/port.d \
./src/queue.d \
./src/sysinit.d \
./src/tasks.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__USE_CMSIS_DSPLIB=CMSIS_DSPLIB_CM3 -DCPP_USE_HEAP -D__LPC17XX__ -I"/home/tarak/Documents/MCUXpresso_10.2.0_759/workspace/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/tarak/Documents/MCUXpresso_10.2.0_759/workspace/lpc_chip_175x_6x/inc" -I"/home/tarak/Documents/MCUXpresso_10.2.0_759/workspace/CMSIS_DSPLIB_CM3/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__USE_CMSIS_DSPLIB=CMSIS_DSPLIB_CM3 -DCPP_USE_HEAP -D__LPC17XX__ -I"/home/tarak/Documents/MCUXpresso_10.2.0_759/workspace/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/tarak/Documents/MCUXpresso_10.2.0_759/workspace/lpc_chip_175x_6x/inc" -I"/home/tarak/Documents/MCUXpresso_10.2.0_759/workspace/CMSIS_DSPLIB_CM3/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


