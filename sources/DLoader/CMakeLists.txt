# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (DLoader)

include_directories(../common/_VS_KEIL
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    src
                    src/Hardware/FatFS
                    src/Hardware/HAL
                    src/Hardware/USBH
                    ..
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
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
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd5246)

file(GLOB LIBS_FATFS     ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff_gen_drv.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/diskio.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/ccsbcs.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/syscall.*)

file(GLOB LIBS_HAL       ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_crc.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash_ex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_spi.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim_ex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_hcd.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_usb.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.*)

file(GLOB LIBS_USBH      src/libs/USBH/*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.*)
                         
file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_FATFS src/Hardware/FatFS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)
file(GLOB HARDWARE_USBH  src/Hardware/USBH/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB COMMON         ../common/Command.*
                         ../common/Common.*
                         ../common/Handlers_d.h
                         ../common/Interface_d.*
                         ../common/Messages.*
                         ../common/Messages_l.*
                         ../common/Transceiver.h
                         ../common/Transceiver_d.cpp)
file(GLOB FDRIVE         src/FDrive/*)
file(GLOB GENERATOR      src/Generator/*)
file(GLOB INTERFACE      src/Interface/*)
file(GLOB SETTINGS       src/Settings/*)
file(GLOB UTILS          src/Utils/*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs/FatFS      FILES ${LIBS_FATFS})
source_group(libs/HAL        FILES ${LIBS_HAL})
source_group(libs/USBH       FILES ${LIBS_USBH})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware/CMSIS  FILES ${HARDWARE_CMSIS})
source_group(Hardware/FatFS  FILES ${HARDWARE_FATFS})
source_group(Hardware/HAL    FILES ${HARDWARE_HAL})
source_group(Hardware/USBH   FILES ${HARDWARE_USBH})
source_group(src             FILES ${SRC})
source_group(common          FILES ${COMMON})
source_group(FDrive          FILES ${FDRIVE})
source_group(Generator       FILES ${GENERATOR})
source_group(Interface       FILES ${INTERFACE})
source_group(Settings        FILES ${SETTINGS})
source_group(Utils           FILES ${UTILS})

                   
add_library(${PROJECT_NAME} ${LIBS_FATFS}
                            ${LIBS_USBH}
                            ${LIBS_HAL}
                            ${HARDWARE}
                            ${HARDWARE_CMSIS}
                            ${HARDWARE_FATFS}
                            ${HARDWARE_HAL}
                            ${HARDWARE_USBH}
                            ${SRC}
                            ${COMMON}
                            ${FDRIVE}
                            ${GENERATOR}
                            ${INTERFACE}
                            ${SETTINGS}
                            ${UTILS}
)
