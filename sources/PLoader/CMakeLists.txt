cmake_minimum_required(VERSION 3.13)

project (PLoader)

include_directories( src
                     src/libs/HAL
                     src/Hardware
                     ..
                     ../common/_VS_KEIL/CMSIS
                     ../common/_VS_KEIL/std_includes
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
add_compile_options(/wd4626)
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4946)    # reinterpret_cast used between related classes: 'Page' and 'Item'
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5027)    # 'xxx': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified


set_source_files_properties(../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                            ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
                            PROPERTIES HEADER_FILE_ONLY TRUE)


file(GLOB COMMON          ../common/Command.h
                          ../common/Command.cpp
                          ../common/Common.h
                          ../common/Common.cpp
                          ../common/Interface_p.h
                          ../common/Messages.h
                          ../common/Messages.cpp
                          ../common/Messages_l.h
                          ../common/Messages_l.cpp
                          ../common/Transceiver.h
                          ../common/Transceiver_p.cpp)
file(GLOB SRC             src/*)
file(GLOB DISPLAY         src/Display/*)
file(GLOB GENERATOR       src/Generator/*)
file(GLOB HARDWARE        src/Hardware/*)
file(GLOB HARDWARE_HAL    src/Hardware/HAL/*)
file(GLOB KEYBOARD        src/Keyboard/*)
file(GLOB INTERFACE       src/Interface/*)
file(GLOB LIBS_CMSIS      src/libs/CMSIS/*)
file(GLOB LIBS_HAL        src/libs/HAL/*)
file(GLOB SETTINGS        src/Settings/*)
file(GLOB UTILS           src/Utils/*)
file(GLOB THIRD_PARTY_HAL ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
                          ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                          ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_crc.h
                          ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c)

source_group(common          FILES ${COMMON})
source_group(src             FILES ${SRC})
source_group(Display         FILES ${DISPLAY})
source_group(Generator       FILES ${GENERATOR})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware\\HAL   FILES ${HARDWARE_HAL})
source_group(Keyboard        FILES ${KEYBOARD})
source_group(Interface       FILES ${INTERFACE})
source_group(libs\\CMSIS     FILES ${LIBS_CMSIS})
source_group(libs\\HAL       FILES ${LIBS_HAL})
source_group(Settings        FILES ${SETTINGS})
source_group(Utils           FILES ${UTILS})
source_group(ThirdParty\\HAL FILES ${THIRD_PARTY_HAL})

add_library(PLoader ${COMMON} ${SRC} ${DISPLAY} ${GENERATOR} ${HARDWARE} ${HARDWARE_HAL} ${KEYBOARD} ${INTERFACE}
                    ${LIBS_CMSIS} ${LIBS_HAL} ${SETTINGS} ${UTILS} ${THIRD_PARTY_HAL})
