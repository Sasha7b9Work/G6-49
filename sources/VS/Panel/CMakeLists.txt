cmake_minimum_required(VERSION 3.13)

project (Panel)

include_directories( ../../Panel/src
                     ../../Panel/src/libs/HAL
                     ../../Panel/src/Hardware
                     ../../common
                     ../../common/_VS
                     ../../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                     ../../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
                     ../../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    )

add_definitions(-DSTM32F429xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201 /wd4514 /wd4623 /wd4774 /wd4820 /wd5039 /wd5045)

source_group(src                      REGULAR_EXPRESSION ../../Panel/src/*.*)
source_group(Display                  REGULAR_EXPRESSION ../../Panel/src/Display/*.*)
source_group(Display\\font            REGULAR_EXPRESSION ../../Panel/src/Display/font/*.*)
source_group(FDrive                   REGULAR_EXPRESSION ../../Panel/src/FDrive/*.*)
source_group(FrequencyMeter           REGULAR_EXPRESSION ../../Panel/src/FrequencyMeter/*.*)
source_group(Generator                REGULAR_EXPRESSION ../../Panel/src/Generator/*.*)
source_group(Hardware                 REGULAR_EXPRESSION ../../Panel/src/Hardware/*.*)
source_group(Hardware\\Modules        REGULAR_EXPRESSION ../../Panel/src/Hardware/Modules/*.*)
source_group(Interface                REGULAR_EXPRESSION ../../Panel/src/Interface/*.*)
source_group(libs\\CMSIS              REGULAR_EXPRESSION ../../Panel/src/libs/CMSIS/*.*)
source_group(libs\\HAL                REGULAR_EXPRESSION ../../Panel/src/libs/HAL/*.*)
source_group(Menu                     REGULAR_EXPRESSION ../../Panel/src/Menu/*.*)
source_group(Menu\\Pages              REGULAR_EXPRESSION ../../Panel/src/Menu/Pages/*.*)
source_group(Menu\\Pages\\Include     REGULAR_EXPRESSION ../../Panel/src/Menu/Pages/Include/*.*)
source_group(Menu\\Pages\\PageSignals REGULAR_EXPRESSION ../../Panel/src/Menu/Pages/PageSignals/*.*)
source_group(Settings                 REGULAR_EXPRESSION ../../Panel/src/Settings/*.*)
source_group(Utils                    REGULAR_EXPRESSION ../../Panel/src/Utils/*.*)

file(GLOB SOURCES   ../../Panel/src/*
                    ../../Panel/src/Display/*
                    ../../Panel/src/Display/font/*
                    ../../Panel/src/FDrive/*
                    ../../Panel/src/FrequencyMeter/*
                    ../../Panel/src/Generator/*
                    ../../Panel/src/Hardware/*
                    ../../Panel/src/Hardware/Modules/*
                    ../../Panel/src/Interface/*
                    ../../Panel/src/libs/CMSIS/startup_stm32f429xx.s
                    ../../Panel/src/libs/HAL/*
                    ../../Panel/src/Menu/*
                    ../../Panel/src/Menu/Pages/*
                    ../../Panel/src/Menu/Pages/Include/*
                    ../../Panel/src/Menu/Pages/PageSignals/*
                    ../../Panel/src/Settings/*
                    ../../Panel/src/Utils/*
                    )

add_library(Panel ${SOURCES})
