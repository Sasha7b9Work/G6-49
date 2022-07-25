# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Panel)

include_directories( src
                     src/Hardware/HAL
                     ..
                     ../common/_VS_KEIL/CMSIS
                     ../common/_VS/std_includes
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                     ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    )

add_definitions(-DSTM32F429xx -DPANEL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4211)    # nonstandard extension used: redefined extern to static
add_compile_options(/wd4355)    # 'this': used in base member initializer list
add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'xxx': default constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4946)    # reinterpret_cast used between related classes: 'Page' and 'Item'
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5027)    # 'xxx': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd5246)
add_compile_options(/wd26812)   # The enum type 'xxx' in unscoped. Prefer 'enum class' over 'enum'


file(GLOB LIBS_HAL             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_crc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma2d.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_ltdc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pwr.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pwr_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_spi.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_sram.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_fmc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dac.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dac_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.*
                               ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.*)

file(GLOB HARDWARE             src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS       src/Hardware/CMSIS/*)
file(GLOB HARDWARE_HAL         src/Hardware/HAL/*
                               ../common/Hardware/HAL/HAL_SPI4_p.cpp)
file(GLOB SRC                  src/*.h src/*.cpp)
file(GLOB COMMON               ../common/Command.*
                               ../common/Common.*
                               ../common/Common_pan.cpp
                               ../common/Interface_p.h
                               ../common/Messages.*
                               ../common/Transceiver.h
                               ../common/Transceiver_p.cpp)
file(GLOB DISPLAY              src/Display/*.h src/Display/*.cpp)
file(GLOB DISPLAY_FONT         src/Display/Font/*)
file(GLOB FDRIVE               src/FDrive/*)
file(GLOB FREQMETER            src/FreqMeter/*)
file(GLOB GENERATOR            src/Generator/*)
file(GLOB KEYBOARD             src/Keyboard/*)
file(GLOB INTERFACE            src/Interface/*)
file(GLOB MENU                 src/Menu/*.h src/Menu/*.cpp)
file(GLOB MENU_PAGES           src/Menu/Pages/*.h src/Menu/Pages/*.cpp)
file(GLOB MENU_PAGES_ADDITION  src/Menu/Pages/Addition/*)
file(GLOB MENU_PAGES_PAGEDEBUG src/Menu/Pages/PageDebug/*)
file(GLOB SCPI                 src/SCPI/*)
file(GLOB SETTINGS             src/Settings/*)
file(GLOB TESTS                src/Tests/*)
file(GLOB UTILS                src/Utils/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\HAL              FILES ${LIBS_HAL})
source_group(Hardware               FILES ${HARDWARE})
source_group(Hardware\\CMSIS        FILES ${HARDWARE_CMSIS})
source_group(Hardware\\HAL          FILES ${HARDWARE_HAL})
source_group(src                    FILES ${SRC})
source_group(common                 FILES ${COMMON})
source_group(Display                FILES ${DISPLAY})
source_group(Display\\Font          FILES ${DISPLAY_FONT})
source_group(FDrive                 FILES ${FDRIVE})
source_group(FreqMeter              FILES ${FREQMETER})
source_group(Generator              FILES ${GENERATOR})
source_group(Interface              FILES ${INTERFACE})
source_group(Keyboard               FILES ${KEYBOARD})
source_group(Menu                   FILES ${MENU})
source_group(Menu\\Pages            FILES ${MENU_PAGES})
source_group(Menu\\Pages\\Addition  FILES ${MENU_PAGES_ADDITION})
source_group(Menu\\Pages\\PageDebug FILES ${MENU_PAGES_PAGEDEBUG})
source_group(SCPI                   FILES ${SCPI})
source_group(Settings               FILES ${SETTINGS})
source_group(Tests                  FILES ${TESTS})
source_group(Utils                  FILES ${UTILS})

add_library(Panel ${LIBS_HAL} ${HARDWARE} ${HARDWARE_CMSIS} ${HARDWARE_HAL} ${SRC} ${COMMON} ${DISPLAY} ${DISPLAY_FONT}
                  ${FDRIVE} ${FREQMETER} ${GENERATOR} ${INTERFACE} ${KEYBOARD} ${MENU} ${MENU_PAGES} ${MENU_PAGES_ADDITION}
                  ${MENU_PAGES_PAGEDEBUG} ${SCPI} ${SETTINGS} ${TESTS} ${UTILS})