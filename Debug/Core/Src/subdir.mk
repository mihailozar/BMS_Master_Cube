################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/CANlibrary.c \
../Core/Src/PwmIn.c \
../Core/Src/bq76pl455qa1.c \
../Core/Src/can.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/master.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_hal_timebase_tim.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/tim.c \
../Core/Src/uart_driver.c \
../Core/Src/usart.c \
../Core/Src/userMain.c 

OBJS += \
./Core/Src/CANlibrary.o \
./Core/Src/PwmIn.o \
./Core/Src/bq76pl455qa1.o \
./Core/Src/can.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/master.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_hal_timebase_tim.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tim.o \
./Core/Src/uart_driver.o \
./Core/Src/usart.o \
./Core/Src/userMain.o 

C_DEPS += \
./Core/Src/CANlibrary.d \
./Core/Src/PwmIn.d \
./Core/Src/bq76pl455qa1.d \
./Core/Src/can.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/master.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_hal_timebase_tim.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/tim.d \
./Core/Src/uart_driver.d \
./Core/Src/usart.d \
./Core/Src/userMain.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

