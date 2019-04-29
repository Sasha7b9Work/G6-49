cmake_minimum_required(VERSION 3.13)

project (Device)

include_directories(../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    src
                    ../common
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../Device/src/libs/USBD
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../Device/src/libs/HAL
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../Device/src/libs/USBH
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ../Device/src/libs/FatFS
                    )

add_definitions(-DSTM32F407xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201 /wd4514 /wd4623 /wd4820 /wd5039 /wd5045)
add_compile_options(/wd4626)
add_compile_options(/wd5027)

source_group(common             REGULAR_EXPRESSION ../common/*.*)
source_group(src                REGULAR_EXPRESSION src/*.*)
source_group(FDrive             REGULAR_EXPRESSION src/FDrive/*.*)
source_group(FreqMeter          REGULAR_EXPRESSION src/FreqMeter/*.*)
source_group(Generator          REGULAR_EXPRESSION src/Generator/*.*)
source_group(Hardware           REGULAR_EXPRESSION src/Hardware/*.*)
source_group(Hardware\\Modules  REGULAR_EXPRESSION src/Hardware/Modules/*.*)
source_group(Interface          REGULAR_EXPRESSION src/Interface/*.*)
source_group(libs               REGULAR_EXPRESSION src/libs/*.*)
source_group(libs\\CMSIS        REGULAR_EXPRESSION src/libs/CMSIS/*.*)
source_group(libs\\FatFS        REGULAR_EXPRESSION src/libs/FatFS/*.*)
source_group(libs\\HAL          REGULAR_EXPRESSION src/libs/HAL/*.*)
source_group(libs\\USBD         REGULAR_EXPRESSION src/libs/USBD/*.*)
source_group(libs\\USBH         REGULAR_EXPRESSION src/libs/USBH/*.*)
source_group(Settings           REGULAR_EXPRESSION src/Settings/*.*)
source_group(Utils              REGULAR_EXPRESSION src/Utils/*.*)

file(GLOB SOURCES   ../common/*
                    src/*
                    src/FDrive/*
                    src/FreqMeter/*
                    src/Generator/*
                    src/Hardware/*
                    src/Hardware/Modules/*
                    src/Interface/*
                    src/libs/*
                    src/libs/CMSIS/startup_stm32f407xx.s
                    src/libs/FatFS/*
                    src/libs/HAL/*
                    src/libs/USBD/*
                    src/libs/USBH/*
                    src/Settings/*
                    src/Utils/*)

list(REMOVE_ITEM SOURCES "src/libs/CMSIS/system_stm32f4xx.c")
                    
add_library(Device ${SOURCES})
