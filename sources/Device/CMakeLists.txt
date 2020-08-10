cmake_minimum_required(VERSION 3.13)

project (Device)

include_directories(../common/_VS_KEIL
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    src
                    ..
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

add_compile_options(/wd4201)
add_compile_options(/wd4514)
add_compile_options(/wd4623)
add_compile_options(/wd4626)
add_compile_options(/wd4820)
add_compile_options(/wd5027)
add_compile_options(/wd5039)
add_compile_options(/wd5045)

file(GLOB COMMON       ../common/Command.h
                       ../common/Command.cpp
                       ../common/Common.h
                       ../common/Common.cpp
                       ../common/Handlers_d.h
                       ../common/Interface_d.h
                       ../common/Interface_d.cpp
                       ../common/Messages.h
                       ../common/Messages.cpp
                       ../common/Transceiver.h
                       ../common/Transceiver_d.cpp)
file(GLOB SRC          src/*)
file(GLOB FDRIVE       src/FDrive/*)
file(GLOB FREQMETER    src/FreqMeter/*)
file(GLOB GENERATOR    src/Generator/*)
file(GLOB HARDWARE     src/Hardware/*)
file(GLOB HARDWARE_HAL src/Hardware/HAL/*)
file(GLOB INTERFACE    src/Interface/*)
file(GLOB LIBS_CMSIS   src/libs/CMSIS/*)
file(GLOB LIBS_FATFS   src/libs/FatFS/*)
file(GLOB LIBS_HAL     src/libs/HAL/*)
file(GLOB LIBS_USBD    src/libs/USBD/*)
file(GLOB LIBS_USBH    src/libs/USBH/*)
file(GLOB SETTINGS     src/Settings/*)
file(GLOB UTILS        src/Utils/*)

source_group(common        FILES ${COMMON})
source_group(src           FILES ${SRC})
source_group(FDrive        FILES ${FDRIVE})
source_group(FreqMeter     FILES ${FREQMETER})
source_group(Generator     FILES ${GENERATOR})
source_group(Hardware      FILES ${HARDWARE})
source_group(Hardware\\HAL FILES ${HARDWARE_HAL})
source_group(Interface     FILES ${INTERFACE})
source_group(libs\\CMSIS   FILES ${LIBS_CMSIS})
source_group(libs\\FatFS   FILES ${LIBS_FATFS})
source_group(libs\\HAL     FILES ${LIBS_HAL})
source_group(libs\\USBD    FILES ${LIBS_USBD})
source_group(libs\\USBH    FILES ${LIBS_USBH})
source_group(Settings      FILES ${SETTINGS})
source_group(Utils         FILES ${UTILS})

add_library(Device ${COMMON} ${SRC} ${FDRIVE} ${FREQMETER} ${GENERATOR} ${HARDWARE} ${HARDWARE_HAL} ${INTERFACE} ${LIBS_CMSIS}
                   ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBD} ${LIBS_USBH} ${SETTINGS} ${UTILS})
