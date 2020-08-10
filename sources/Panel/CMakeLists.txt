cmake_minimum_required(VERSION 3.13)

project (Panel)

include_directories( src
                     src/libs/HAL
                     src/Hardware
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
add_compile_options(/wd26812)   # The enum type 'xxx' in unscoped. Prefer 'enum class' over 'enum'


file(GLOB COMMON               ../common/Command.h
                               ../common/Command.cpp
                               ../common/Common.h
                               ../common/Common.cpp
                               ../common/Common_pan.cpp
                               ../common/Interface_p.h
                               ../common/Messages.h
                               ../common/Messages.cpp
                               ../common/Transceiver.h
                               ../common/Transceiver_p.cpp)
file(GLOB SRC                  src/*)
file(GLOB DISPLAY              src/Display/*)
file(GLOB DISPLAY_FONT         src/Display/Font/*)
file(GLOB FDRIVE               src/FDrive/*)
file(GLOB FREQMETER            src/FreqMeter/*)
file(GLOB GENERATOR            src/Generator/*)
file(GLOB HARDWARE             src/Hardware/*)
file(GLOB HARDWARE_HAL         src/Hardware/HAL/HAL.h
                               src/Hardware/HAL/HAL.cpp
                               src/Hardware/HAL/HAL_CRC32.cpp
                               src/Hardware/HAL/HAL_EEPROM.cpp
                               src/Hardware/HAL/HAL_IT.cpp
                               src/Hardware/HAL/HAL_LTDC.cpp
                               src/Hardware/HAL/HAL_PIO.h
                               src/Hardware/HAL/HAL_PIO.cpp
                               src/Hardware/HAL/HAL_TIM.cpp
                               src/Hardware/HAL/HAL_TIM2.cpp
                               src/Hardware/HAL/HAL_TIM3.cpp
                               src/Hardware/HAL/HAL_TIM4.cpp)
file(GLOB KEYBOARD             src/Keyboard/*)
file(GLOB INTERFACE            src/Interface/*)
file(GLOB LIBS_CMSIS           src/libs/CMSIS/*)
file(GLOB LIBS_HAL             src/libs/HAL/*)
file(GLOB MENU                 src/Menu/*)
file(GLOB MENU_PAGES           src/Menu/Pages/*)
file(GLOB MENU_PAGES_ADDITION  src/Menu/Pages/Addition/*)
file(GLOB MENU_PAGES_PAGEDEBUG src/Menu/Pages/PageDebug/*)
file(GLOB _SCPI                src/SCPI/*)
file(GLOB SETTINGS             src/Settings/*)
file(GLOB UTILS                src/Utils/*)

source_group(common                 FILES ${COMMON})
source_group(src                    FILES ${SRC})
source_group(Display                FILES ${DISPLAY})
source_group(Display\\Font          FILES ${DISPLAY_FONT})
source_group(FDrive                 FILES ${FDRIVE})
source_group(FreqMeter              FILES ${FREQMETER})
source_group(Generator              FILES ${GENERATOR})
source_group(Hardware               FILES ${HARDWARE})
source_group(Hardware\\HAL          FILES ${HARDWARE_HAL})
source_group(Keyboard               FILES ${KEYBOARD})
source_group(Interface              FILES ${INTERFACE})
source_group(libs\\CMSIS            FILES ${LIBS_CMSIS})
source_group(libs\\HAL              FILES ${LIBS_HAL})
source_group(Menu                   FILES ${MENU})
source_group(Menu\\Pages            FILES ${MENU_PAGES})
source_group(Menu\\Pages\\Addition  FILES ${MENU_PAGES_ADDITION})
source_group(Menu\\Pages\\PageDebug FILES ${MENU_PAGES_PAGEDEBUG})
source_group(SCPI                   FILES ${_SCPI})
source_group(Settings               FILES ${SETTINGS})
source_group(Utils                  FILES ${UTILS})

add_library(Panel ${COMMON} ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${FDRIVE} ${FREQMETER} ${GENERATOR} ${HARDWARE} ${HARDWARE_HAL} ${KEYBOARD}
                  ${INTERFACE} ${LIBS_CMSIS} ${LIBS_HAL} ${MENU} ${MENU_PAGES} ${MENU_PAGES_ADDITION} ${MENU_PAGES_PAGEDEBUG} ${_SCPI} ${SETTINGS} ${UTILS})
