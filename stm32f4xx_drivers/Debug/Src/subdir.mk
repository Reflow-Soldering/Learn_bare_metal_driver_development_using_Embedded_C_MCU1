################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/016.UART_CASE.c \
../Src/sysmem.c 

OBJS += \
./Src/016.UART_CASE.o \
./Src/sysmem.o 

C_DEPS += \
./Src/016.UART_CASE.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"G:/Git_Learn_bare_metal_driver_development_using_Embedded_C_MCU1/stm32f4xx_drivers/drivers/inc" -I"H:/Git_Learn_bare_metal_driver_development_using_Embedded_C_MCU1/stm32f4xx_drivers/drivers/inc" -I"C:/repo/Learn_bare_metal_driver_development_using_Embedded_C_MCU1/stm32f4xx_drivers/drivers/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/016.UART_CASE.cyclo ./Src/016.UART_CASE.d ./Src/016.UART_CASE.o ./Src/016.UART_CASE.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

