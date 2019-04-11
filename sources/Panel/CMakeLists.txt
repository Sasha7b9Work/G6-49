cmake_minimum_required(VERSION 3.13)

project (Panel)

include_directories( src
                     src/libs/HAL
                     src/Hardware
                     ../common
                     ../common/_VS_KEIL/CMSIS
#                     ../common/_VS/HAL
                     ../common/_VS/std_includes
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                     ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                     ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    )

add_definitions(-DSTM32F429xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument N is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4996)    # This function or variable may be unsafe
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

source_group(common                   REGULAR_EXPRESSION ../common/*.*)
source_group(src                      REGULAR_EXPRESSION src/*.*)
source_group(Display                  REGULAR_EXPRESSION src/Display/*.*)
source_group(Display\\font            REGULAR_EXPRESSION src/Display/font/*.*)
source_group(FDrive                   REGULAR_EXPRESSION src/FDrive/*.*)
source_group(FrequencyMeter           REGULAR_EXPRESSION src/FrequencyMeter/*.*)
source_group(Generator                REGULAR_EXPRESSION src/Generator/*.*)
source_group(Hardware                 REGULAR_EXPRESSION src/Hardware/*.*)
source_group(Hardware\\Modules        REGULAR_EXPRESSION src/Hardware/Modules/*.*)
source_group(Interface                REGULAR_EXPRESSION src/Interface/*.*)
source_group(libs\\CMSIS              REGULAR_EXPRESSION src/libs/CMSIS/*.*)
source_group(libs\\HAL                REGULAR_EXPRESSION src/libs/HAL/*.*)
source_group(Menu                     REGULAR_EXPRESSION src/Menu/*.*)
source_group(Menu\\Pages              REGULAR_EXPRESSION src/Menu/Pages/*.*)
source_group(Menu\\Pages\\Include     REGULAR_EXPRESSION src/Menu/Pages/Include/*.*)
source_group(Menu\\Pages\\PageSignals REGULAR_EXPRESSION src/Menu/Pages/PageSignals/*.*)
source_group(Settings                 REGULAR_EXPRESSION src/Settings/*.*)
source_group(Utils                    REGULAR_EXPRESSION src/Utils/*.*)

file(GLOB SOURCES   ../common/*
                    src/*
                    src/Display/*
                    src/Display/font/*
                    src/FDrive/*
                    src/FrequencyMeter/*
                    src/Generator/*
                    src/Hardware/*
                    src/Hardware/Modules/*
                    src/Interface/*
                    src/libs/CMSIS/startup_stm32f429xx.s
                    src/libs/HAL/*
                    src/Menu/*
                    src/Menu/Pages/*
                    src/Menu/Pages/Include/*
                    src/Menu/Pages/PageSignals/*
                    src/Settings/*
                    src/Utils/*
                    )

add_library(Panel ${SOURCES})
