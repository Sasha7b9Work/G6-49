cmake_minimum_required(VERSION 3.0)

project (Panel)

include_directories("..\\..\\Panel\\src\\"
                    "..\\..\\Panel\\src\\libs\\HAL\\"
                    "..\\..\\Panel\\src\\Hardware\\"
                    "..\\..\\common\\"
                    "..\\..\\common\\_VS\\"
                    "..\\..\\common\\_VS\\CMSIS\\"
                    "..\\..\\common\\_VS\\HAL\\"
                    "..\\..\\Panel\\src\\libs\\HAL"
                    "..\\..\\ThirdParty\\SDL2\\SDL2-2.0.9\\include\\")

add_definitions(-DSTM32F437xx -DWXUSINGDLL -DUSE_SDL2)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201 /wd4514 /wd4623 /wd4774 /wd4820 /wd5039 /wd5045)
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4996)    # function or variable may be unsafe

source_group(common                   FILES              ../../common/Command.cpp
                                                         ../../common/Command.h
                                                         ../../common/Common.cpp
                                                         ../../common/Common.h
                                                         ../../common/CommonTypes.h
                                                         ../../common/Message.cpp
                                                         ../../common/Message.h
                                                         ../../common/Transceiver.cpp
                                                         ../../common/Transceiver.h
                                                         ../../common/Wave.h)
source_group(common\\_VS\\HAL         FILES              ../../common/_VS/HAL/stm32f4xx_hal.cpp)
source_group(src                      REGULAR_EXPRESSION ../Panel/src/*.*)
source_group(Display                  REGULAR_EXPRESSION ../Panel/src/Display/*.*)
source_group(Display\\font            REGULAR_EXPRESSION ../Panel/src/Display/font/*.*)
source_group(FDrive                   REGULAR_EXPRESSION ../Panel/src/FDrive/*.*)
source_group(FrequencyMeter           REGULAR_EXPRESSION ../Panel/src/FrequencyMeter/*.*)
source_group(Generator                REGULAR_EXPRESSION ../Panel/src/Generator/*.*)
source_group(Hardware                 FILES              ../../Panel/src/Hardware/Controls.cpp
                                                         ../../Panel/src/Hardware/Controls.h
                                                         ../../Panel/src/Hardware/CPU.h
                                                                     src/Hardware/CPU_win.cpp
                                                                     src/Hardware/CPU_Keyboard_win.cpp
                                                         ../../Panel/src/Hardware/LTDC.h
                                                                     src/Hardware/LTDC_win.cpp
                                                         ../../Panel/src/Hardware/stm429.h
                                                         ../../Panel/src/Hardware/Timer.cpp
                                                         ../../Panel/src/Hardware/Timer.h
                                                         ../../Panel/src/Hardware/Timer4XX.h
                                                                     src/Hardware/Timer4XX_win.cpp
                                                         ../../Panel/src/Hardware/VCP.cpp
                                                         ../../Panel/src/Hardware/VCP.h)
source_group(Hardware\\Modules        FILES              ../../Panel/src/Hardware/Modules/EEPROM.h
                                                                     src/Hardware/Modules/EEPROM_win.cpp
                                                         ../../Panel/src/Hardware/Modules/SPI.h)
                                                         
source_group(Interface                FILES              ../../Panel/src/Interface/HandlersPanel.cpp
                                                         ../../Panel/src/Interface/HandlersPanel.h
                                                         ../../Panel/src/Interface/InterfacePanel.cpp
                                                         ../../Panel/src/Interface/InterfacePanel.h
                                                                     src/Interface/Transceiver_429_win.cpp)
source_group(libs\\CMSIS              REGULAR_EXPRESSION ../Panel/src/libs/CMSIS/*.*)
source_group(libs\\HAL                REGULAR_EXPRESSION ../Panel/src/libs/HAL/*.*)
source_group(Menu                     REGULAR_EXPRESSION ../Panel/src/Menu/*.*)
source_group(Menu\\Pages              REGULAR_EXPRESSION ../Panel/src/Menu/Pages/*.*)
source_group(Menu\\Pages\\Include     REGULAR_EXPRESSION ../Panel/src/Menu/Pages/Include/*.*)
source_group(Menu\\Pages\\PageSignals REGULAR_EXPRESSION ../Panel/src/Menu/Pages/PageSignals/*.*)
source_group(Settings                 REGULAR_EXPRESSION ../Panel/src/Settings/*.*)
source_group(Utils                    REGULAR_EXPRESSION ../Panel/src/Utils/*.*)

file(GLOB SOURCES   ../../common/Command.cpp
                    ../../common/Command.h
                    ../../common/Common.cpp
                    ../../common/Common.h
                    ../../common/CommonTypes.h
                    ../../common/Message.cpp
                    ../../common/Message.h
                    ../../common/Transceiver.cpp
                    ../../common/Transceiver.h
                    ../../common/Wave.h
                                                         
                    ../../common/_VS/HAL/stm32f4xx_hal.cpp
                    ../../Panel/src/*
                    ../../Panel/src/Display/*
                    ../../Panel/src/Display/font/*
                    ../../Panel/src/FDrive/*
                    ../../Panel/src/FrequencyMeter/*
                    ../../Panel/src/Generator/*
                    
                    ../../Panel/src/Hardware/Controls.cpp
                    ../../Panel/src/Hardware/Controls.h
                    ../../Panel/src/Hardware/CPU.h
                                src/Hardware/CPU_win.cpp
                                src/Hardware/CPU_Keyboard_win.cpp
                    ../../Panel/src/Hardware/LTDC.h
                                src/Hardware/LTDC_win.cpp
                    ../../Panel/src/Hardware/stm429.h
                    ../../Panel/src/Hardware/Timer.cpp
                    ../../Panel/src/Hardware/Timer.h
                    ../../Panel/src/Hardware/Timer4XX.h
                                src/Hardware/Timer4XX_win.cpp
                    ../../Panel/src/Hardware/VCP.cpp
                    ../../Panel/src/Hardware/VCP.h
                    
                    ../../Panel/src/Hardware/Modules/EEPROM.h
                                src/Hardware/Modules/EEPROM_win.cpp
                    ../../Panel/src/Hardware/Modules/SPI.h
                    
                    ../../Panel/src/Interface/HandlersPanel.cpp
                    ../../Panel/src/Interface/HandlersPanel.h
                    ../../Panel/src/Interface/InterfacePanel.cpp
                    ../../Panel/src/Interface/InterfacePanel.h
                                src/Interface/Transceiver_429_win.cpp
                    
                    ../../Panel/src/libs/CMSIS/startup_stm32f429xx.s
                    ../../Panel/src/libs/HAL/*
                    ../../Panel/src/Menu/*
                    ../../Panel/src/Menu/Pages/*
                    ../../Panel/src/Menu/Pages/Include/*
                    ../../Panel/src/Menu/Pages/PageSignals/*
                    ../../Panel/src/Settings/*
                    ../../Panel/src/Utils/*
                    )

add_executable(Panel ${SOURCES})

target_link_libraries(Panel "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxbase31ud.lib")
target_link_libraries(Panel "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxmsw31ud_core.lib")

target_link_libraries(Panel "..\\..\\..\\ThirdParty\\SDL2\\SDL2-2.0.9\\generated\\Debug\\SDL2-staticd.lib")

target_link_libraries(Panel WinMM.lib)
target_link_libraries(Panel version.lib)
target_link_libraries(Panel Imm32.Lib)
