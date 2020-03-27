cmake_minimum_required(VERSION 3.13)

project (PLoader)

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
add_compile_options(/wd4626)
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4946)    # reinterpret_cast used between related classes: 'Page' and 'Item'
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5027)    # 'xxx': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

source_group(common                   FILES ../common/Command.h
                                            ../common/Command.cpp
                                            ../common/Common.h
                                            ../common/Common.cpp
                                            ../common/Messages.h
                                            ../common/Messages.cpp
                                            ../common/Transceiver.h)
source_group(src                      REGULAR_EXPRESSION src/*)
source_group(FDrive                   REGULAR_EXPRESSION src/FDrive/*)
source_group(Generator                REGULAR_EXPRESSION src/Generator/*)
source_group(Hardware                 REGULAR_EXPRESSION src/Hardware/*)
source_group(Hardware\\HAL            REGULAR_EXPRESSION src/Hardware/HAL/*)
source_group(Keyboard                 REGULAR_EXPRESSION src/Keyboard/*)
source_group(Interface                REGULAR_EXPRESSION src/Interface/*)
source_group(libs\\CMSIS              REGULAR_EXPRESSION src/libs/CMSIS/*)
source_group(libs\\HAL                REGULAR_EXPRESSION src/libs/HAL/*)
source_group(Settings                 REGULAR_EXPRESSION src/Settings/*)
source_group(Utils                    REGULAR_EXPRESSION src/Utils/*)

file(GLOB SOURCES   ../common/Command.h
                    ../common/Command.cpp
                    ../common/Common.h
                    ../common/Common.cpp
                    ../common/Messages.h
                    ../common/Messages.cpp
                    ../common/Transceiver.h
                    src/*
                    src/FDrive/*
                    src/Generator/*
                    src/Hardware/*
                    src/Hardware/HAL/*
                    src/Keyboard/*
                    src/Interface/*
                    src/libs/CMSIS/startup_stm32f429xx.s
                    src/libs/HAL/*
                    src/Settings/*
                    src/Utils/*
                    )

add_library(PLoader ${SOURCES})
