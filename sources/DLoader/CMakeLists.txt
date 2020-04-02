cmake_minimum_required(VERSION 3.13)

project (DLoader)

include_directories(../common/_VS_KEIL
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    src
                    ..
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../DLoader/src/libs/USBD
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../DLoader/src/libs/HAL
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../DLoader/src/libs/USBH
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ../DLoader/src/libs/FatFS
                    )

add_definitions(-DSTM32F407xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201)
add_compile_options(/wd4514)
add_compile_options(/wd4623)
add_compile_options(/wd4626)
add_compile_options(/wd4820)
add_compile_options(/wd5027)
add_compile_options(/wd5039)
add_compile_options(/wd5045)


set_source_files_properties(../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                            PROPERTIES HEADER_FILE_ONLY TRUE)


source_group(common             FILES ../common/Command.h
                                      ../common/Command.cpp
                                      ../common/Common.h
                                      ../common/Common.cpp
                                      ../common/Messages.h
                                      ../common/Messages.cpp
                                      ../common/Messages_pl.h
                                      ../common/Messages_pl.cpp
                                      ../common/Transceiver.h)
source_group(src                REGULAR_EXPRESSION src/*)
source_group(FDrive             REGULAR_EXPRESSION src/FDrive/*)
source_group(Generator          REGULAR_EXPRESSION src/Generator/*)
source_group(Hardware           REGULAR_EXPRESSION src/Hardware/*)
source_group(Hardware\\HAL      REGULAR_EXPRESSION src/Hardware/HAL/*)
source_group(Interface          REGULAR_EXPRESSION src/Interface/*)
source_group(libs               REGULAR_EXPRESSION src/libs/*)
source_group(libs\\CMSIS        REGULAR_EXPRESSION src/libs/CMSIS/*)
source_group(libs\\FatFS        REGULAR_EXPRESSION src/libs/FatFS/*)
source_group(libs\\HAL          REGULAR_EXPRESSION src/libs/HAL/*)
source_group(libs\\USBH         REGULAR_EXPRESSION src/libs/USBH/*)
source_group(Settings           REGULAR_EXPRESSION src/Settings/*)
source_group(Utils              REGULAR_EXPRESSION src/Utils/*)
source_group(ThirdParty\\HAL    FILES ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
                                      ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c)

file(GLOB SOURCES   ../common/Command.h
                    ../common/Command.cpp
                    ../common/Common.h
                    ../common/Common.cpp
                    ../common/Messages.h
                    ../common/Messages.cpp
                    ../common/Messages_pl.h
                    ../common/Messages_pl.cpp
                    ../common/Transceiver.h
                    src/*
                    src/FDrive/*
                    src/Generator/*
                    src/Hardware/*
                    src/Hardware/HAL/*
                    src/Interface/*
                    src/libs/*
                    src/libs/CMSIS/startup_stm32f407xx.s
                    src/libs/FatFS/*
                    src/libs/HAL/*
                    src/libs/USBH/*
                    src/Settings/*
                    src/Utils/*
                    
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                    )

list(REMOVE_ITEM SOURCES "src/libs/CMSIS/system_stm32f4xx.c")
                    
add_library(DLoader ${SOURCES})
