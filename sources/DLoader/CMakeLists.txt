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

add_definitions(-DSTM32F407xx -DLOADER)

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
                            ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
                            PROPERTIES HEADER_FILE_ONLY TRUE)

file(GLOB COMMON         ../common/Command.h
                         ../common/Command.cpp
                         ../common/Common.h
                         ../common/Common.cpp
                         ../common/Handlers_d.h
                         ../common/Interface_d.h
                         ../common/Interface_d.cpp
                         ../common/Messages.h
                         ../common/Messages.cpp
                         ../common/Messages_l.h
                         ../common/Messages_l.cpp
                         ../common/Transceiver.h
                         ../common/Transceiver_d.cpp)
file(GLOB SRC            src/*)
file(GLOB FDRIVE         src/FDrive/*)
file(GLOB GENERATOR      src/Generator/*)
file(GLOB HARDWARE       src/Hardware/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)
file(GLOB INTERFACE      src/Interface/*)
file(GLOB LIBS_CMSIS     src/libs/CMSIS/*)
file(GLOB LIBS_FATFS     src/libs/FatFS/*)
file(GLOB LIBS_HAL       src/libs/HAL/*)
file(GLOB LIBS_USBH      src/libs/USBH/*)
file(GLOB SETTINGS       src/Settings/*)
file(GLOB UTILS          src/Utils/*)
file(GLOB THIRDPARTY_HAL ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_crc.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c)

source_group(common          FILES ${COMMON})
source_group(src             FILES ${SRC})
source_group(FDrive          FILES ${FDRIVE})
source_group(Generator       FILES ${GENERATOR})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware\\HAL   FILES ${HARDWARE_HAL})
source_group(Interface       FILES ${INTERFACE})
source_group(libs\\CMSIS     FILES ${LIBS_CMSIS})
source_group(libs\\FatFS     FILES ${LIBS_FATFS})
source_group(libs\\USBH      FILES ${LIBS_USBH})
source_group(Settings        FILES ${SETTINGS})
source_group(Utils           FILES ${UTILS})
source_group(ThirdParty\\HAL FILES ${THIRDPARTY_HAL})

                    
add_library(DLoader ${COMMON} ${SRC} ${FDRIVE} ${GENERATOR} ${HARDWARE} ${HARDWARE_HAL} ${INTERFACE} ${LIBS_CMSIS} ${LIBS_FATFS}
                    ${LIBS_USBH} ${SETTINGS} ${UTILS} ${THIRDPARTY_HAL})
