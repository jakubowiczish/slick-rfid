################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rfid_folder/dbgu.c \
../rfid_folder/rfid.c \
../rfid_folder/term_io.c 

OBJS += \
./rfid_folder/dbgu.o \
./rfid_folder/rfid.o \
./rfid_folder/term_io.o 

C_DEPS += \
./rfid_folder/dbgu.d \
./rfid_folder/rfid.d \
./rfid_folder/term_io.d 


# Each subdirectory must supply rules for building sources it contributes
rfid_folder/dbgu.o: ../rfid_folder/dbgu.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DDEBUG -c -I../USB_HOST/App -I../TouchGFX/gui/include -I../TouchGFX/generated/fonts/include -I../Drivers/BSP/Components/ts3510 -I../TouchGFX/generated/texts/include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/Inc -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/BSP/Components/exc7200 -I../Drivers/BSP/Components/stmpe811 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/otm8009a -I../FATFS/Target -I../Drivers/BSP/Components/Common -I../USB_HOST/Target -I../Middlewares/Third_Party/FatFs/src -I../Drivers/BSP/Components/ampire480272 -I../TouchGFX/target -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/rfid_folder" -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/STM32469I-Discovery" -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/Drivers/BSP/Components/ft6x06" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"rfid_folder/dbgu.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
rfid_folder/rfid.o: ../rfid_folder/rfid.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DDEBUG -c -I../USB_HOST/App -I../TouchGFX/gui/include -I../TouchGFX/generated/fonts/include -I../Drivers/BSP/Components/ts3510 -I../TouchGFX/generated/texts/include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/Inc -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/BSP/Components/exc7200 -I../Drivers/BSP/Components/stmpe811 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/otm8009a -I../FATFS/Target -I../Drivers/BSP/Components/Common -I../USB_HOST/Target -I../Middlewares/Third_Party/FatFs/src -I../Drivers/BSP/Components/ampire480272 -I../TouchGFX/target -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/rfid_folder" -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/STM32469I-Discovery" -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/Drivers/BSP/Components/ft6x06" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"rfid_folder/rfid.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
rfid_folder/term_io.o: ../rfid_folder/term_io.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F469xx -DDEBUG -c -I../USB_HOST/App -I../TouchGFX/gui/include -I../TouchGFX/generated/fonts/include -I../Drivers/BSP/Components/ts3510 -I../TouchGFX/generated/texts/include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../TouchGFX/generated/images/include -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/Inc -I../Middlewares/ST/TouchGFX/touchgfx/framework/include -I../TouchGFX/generated/gui_generated/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../FATFS/App -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/BSP/Components/exc7200 -I../Drivers/BSP/Components/stmpe811 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/otm8009a -I../FATFS/Target -I../Drivers/BSP/Components/Common -I../USB_HOST/Target -I../Middlewares/Third_Party/FatFs/src -I../Drivers/BSP/Components/ampire480272 -I../TouchGFX/target -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/rfid_folder" -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/STM32469I-Discovery" -I"C:/Users/Kasper/Documents/AGH/Wbudowane/rfid_project/Drivers/BSP/Components/ft6x06" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"rfid_folder/term_io.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

