project (Device)

include_directories(../common/_VS_KEIL
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    src
                    ..
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../Device/src/Hardware/USBD
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../Device/src/Hardware/HAL
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../Device/src/Hardware/USBH
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ../Device/src/Hardware/FatFS
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

file(GLOB LIBS_FATFS     ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff_gen_drv.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/diskio.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/ccsbcs.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/syscall.*)
                       
file(GLOB LIBS_HAL       ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_crc.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash_ex.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_i2c.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pcd.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pcd_ex.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_spi.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim_ex.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_nor.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_hcd.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_usb.h
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c)

file(GLOB LIBS_USBD      ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ctlreq.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ioreq.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/usbd_cdc.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c)

file(GLOB LIBS_USBH      ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.h
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c)                       

file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_FATFS src/Hardware/FatFS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)
file(GLOB HARDWARE_USBD  src/Hardware/USBD/*)
file(GLOB HARDWARE_USBH  src/Hardware/USBH/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB COMMON         ../common/Command.*
                         ../common/Common.*
                         ../common/Handlers_d.h
                         ../common/Interface_d.*
                         ../common/Messages.*
                         ../common/Transceiver.h
                         ../common/Transceiver_d.cpp)
file(GLOB FDRIVE         src/FDrive/*)
file(GLOB FREQMETER      src/FreqMeter/*)
file(GLOB GENERATOR      src/Generator/*)
file(GLOB INTERFACE      src/Interface/*)
file(GLOB SETTINGS       src/Settings/*)
file(GLOB UTILS          src/Utils/*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBD} ${LIBS_USBH} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\FatFS     FILES ${LIBS_FATFS})
source_group(libs\\HAL       FILES ${LIBS_HAL})
source_group(libs\\USBD      FILES ${LIBS_USBD})
source_group(libs\\USBH      FILES ${LIBS_USBH})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware\\CMSIS FILES ${HARDWARE_CMSIS})
source_group(Hardware\\FatFS FILES ${HARDWARE_FATFS})
source_group(Hardware\\HAL   FILES ${HARDWARE_HAL})
source_group(Hardware\\USBD  FILES ${HARDWARE_USBD})
source_group(Hardware\\USBH  FILES ${HARDWARE_USBH})
source_group(src             FILES ${SRC})
source_group(common          FILES ${COMMON})
source_group(FDrive          FILES ${FDRIVE})
source_group(FreqMeter       FILES ${FREQMETER})
source_group(Generator       FILES ${GENERATOR})
source_group(Interface       FILES ${INTERFACE})
source_group(Settings        FILES ${SETTINGS})
source_group(Utils           FILES ${UTILS})

add_library(Device ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBD} ${LIBS_USBH} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_FATFS}
                   ${HARDWARE_HAL} ${HARDWARE_USBD} ${HARDWARE_USBH} ${SRC} ${COMMON} ${FDRIVE} ${FREQMETER} ${GENERATOR}
                   ${INTERFACE} ${SETTINGS} ${UTILS})
