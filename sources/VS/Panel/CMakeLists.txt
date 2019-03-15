cmake_minimum_required(VERSION 3.0)

project (Panel)

include_directories("..\\..\\Panel\\src\\"
                    "..\\..\\Panel\\src\\libs\\HAL\\"
                    "..\\..\\Panel\\src\\Hardware\\"
                    "..\\..\\common\\"
                    "..\\..\\common\\_VS\\"
                    "..\\..\\common\\_VS\\CMSIS\\"
                    "..\\..\\common\\_VS\\HAL\\")

add_definitions(-DSTM32F437xx -DWXUSINGDLL -DUSE_SDL2)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_compile_options(/wd4201 /wd4514 /wd4623 /wd4774 /wd4820 /wd5039 /wd5045)
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4996)    # function or variable may be unsafe

source_group(src                      REGULAR_EXPRESSION ../Panel/src/*.*)
source_group(Display                  REGULAR_EXPRESSION ../Panel/src/Display/*.*)
source_group(Display\\font            REGULAR_EXPRESSION ../Panel/src/Display/font/*.*)
source_group(FDrive                   REGULAR_EXPRESSION ../Panel/src/FDrive/*.*)
source_group(FrequencyMeter           REGULAR_EXPRESSION ../Panel/src/FrequencyMeter/*.*)
source_group(Generator                REGULAR_EXPRESSION ../Panel/src/Generator/*.*)
source_group(Hardware                 REGULAR_EXPRESSION ../Panel/src/Hardware/*.*)
source_group(Hardware\\Modules        REGULAR_EXPRESSION ../Panel/src/Hardware/Modules/*.*)
source_group(Interface                REGULAR_EXPRESSION ../Panel/src/Interface/*.*)
source_group(libs\\CMSIS              REGULAR_EXPRESSION ../Panel/src/libs/CMSIS/*.*)
source_group(libs\\HAL                REGULAR_EXPRESSION ../Panel/src/libs/HAL/*.*)
source_group(Menu                     REGULAR_EXPRESSION ../Panel/src/Menu/*.*)
source_group(Menu\\Pages              REGULAR_EXPRESSION ../Panel/src/Menu/Pages/*.*)
source_group(Menu\\Pages\\Include     REGULAR_EXPRESSION ../Panel/src/Menu/Pages/Include/*.*)
source_group(Menu\\Pages\\PageSignals REGULAR_EXPRESSION ../Panel/src/Menu/Pages/PageSignals/*.*)
source_group(Settings                 REGULAR_EXPRESSION ../Panel/src/Settings/*.*)
source_group(Utils                    REGULAR_EXPRESSION ../Panel/src/Utils/*.*)

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

#target_link_libraries(Panel49 "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxbase31ud.lib")
#target_link_libraries(Panel49 "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxmsw31ud_core.lib")
#
#target_link_libraries(Panel49 "..\\..\\..\\ThirdParty\\SDL2\\SDL2-2.0.9\\generated\\Debug\\SDL2-staticd.lib")
#
#target_link_libraries(Panel49 WinMM.lib)
#target_link_libraries(Panel49 version.lib)
#target_link_libraries(Panel49 Imm32.Lib)
