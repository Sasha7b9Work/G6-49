cmake_minimum_required(VERSION 3.0)

project (G6-49)

include_directories(src
                    ../../Panel/src
                    ../../Panel/src/libs/HAL
                    ../../Panel/src/Hardware
                    ../../common
                    ../../common/_VS_GUI
                    ../../common/_VS_GUI/CMSIS
                    ../../common/_VS_GUI/HAL
                    ../../Panel/src/libs/HAL
                    ../../../../ThirdParty/wxWidgets/include
                    ../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)

add_definitions(-DSTM32F437xx -DWXUSINGDLL -DUSE_SDL2 -DPANEL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4355)    # used in base member initializer list
add_compile_options(/wd4211)    # nonstandard extension used: redefined extern to static
add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument 3 is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4946)    # reinterpret_cast used between related classes: 'Page' and 'Item'
add_compile_options(/wd4996)    # function or variable may be unsafe
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

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
source_group(common\\_VS\\HAL         FILES              ../../common/_VS_GUI/HAL/stm32f4xx_hal.cpp)
source_group(src                      FILES              ../../Panel/src/defines.h
                                                         ../../Panel/src/Log.cpp
                                                         ../../Panel/src/Log.h
                                                         ../../Panel/src/stdafx.cpp
                                                         ../../Panel/src/stdafx.h
                                                         ../../Panel/src/structs.h)
source_group(Display                  FILES              ../../Panel/src/Display/Colors.cpp
                                                         ../../Panel/src/Display/Colors.h
                                                         ../../Panel/src/Display/Console.cpp
                                                         ../../Panel/src/Display/Console.h
                                                         ../../Panel/src/Display/Display.cpp
                                                         ../../Panel/src/Display/Display.h
                                                         ../../Panel/src/Display/DisplayTypes.h
                                                         ../../Panel/src/Display/InputWindow.cpp
                                                         ../../Panel/src/Display/InputWindow.h
                                                         ../../Panel/src/Display/InputWindowStruct.cpp
                                                         ../../Panel/src/Display/Painter.cpp
                                                         ../../Panel/src/Display/Painter.h
                                                                     src/Display/Painter_win.cpp
                                                         ../../Panel/src/Display/Symbols.h
                                                         ../../Panel/src/Display/Text.cpp
                                                         ../../Panel/src/Display/Text.h)                                                                     
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
source_group(Interface                FILES              ../../Panel/src/Interface/HandlersPanel.cpp
                                                         ../../Panel/src/Interface/HandlersPanel.h
                                                         ../../Panel/src/Interface/InterfacePanel.cpp
                                                         ../../Panel/src/Interface/InterfacePanel.h
                                                                     src/Interface/Transceiver_429_win.cpp)
source_group(Display\\font            REGULAR_EXPRESSION ../Panel/src/Display/font/*)
source_group(FDrive                   REGULAR_EXPRESSION ../Panel/src/FDrive/*)
source_group(FrequencyMeter           REGULAR_EXPRESSION ../Panel/src/FrequencyMeter/*)
source_group(Generator                REGULAR_EXPRESSION ../Panel/src/Generator/*)
source_group(GUI                      REGULAR_EXPRESSION src/GUI/*)
source_group(libs\\CMSIS              REGULAR_EXPRESSION ../Panel/src/libs/CMSIS/*)
source_group(libs\\HAL                REGULAR_EXPRESSION ../Panel/src/libs/HAL/*)
source_group(Menu                     REGULAR_EXPRESSION ../Panel/src/Menu/*)
source_group(Menu\\Pages              REGULAR_EXPRESSION ../Panel/src/Menu/Pages/*)
source_group(Menu\\Pages\\Addition    REGULAR_EXPRESSION ../Panel/src/Menu/Pages/Addition/*)
source_group(Menu\\Pages\\Include     REGULAR_EXPRESSION ../Panel/src/Menu/Pages/Include/*)
source_group(Menu\\Pages\\PageSignals REGULAR_EXPRESSION ../Panel/src/Menu/Pages/PageSignals/*)
source_group(SCPI                     REGULAR_EXPRESSION ../Panel/src/SCPI/*)
source_group(Settings                 REGULAR_EXPRESSION ../Panel/src/Settings/*)
source_group(Utils                    REGULAR_EXPRESSION ../Panel/src/Utils/*)

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
                    ../../common/_VS_GUI/HAL/stm32f4xx_hal.cpp
                    ../../Panel/src/defines.h
                    ../../Panel/src/Log.cpp
                    ../../Panel/src/Log.h
                    ../../Panel/src/stdafx.cpp
                    ../../Panel/src/stdafx.h
                    ../../Panel/src/structs.h
                    ../../Panel/src/Display/Colors.cpp
                    ../../Panel/src/Display/Colors.h
                    ../../Panel/src/Display/Console.cpp
                    ../../Panel/src/Display/Console.h
                    ../../Panel/src/Display/Display.cpp
                    ../../Panel/src/Display/Display.h
                    ../../Panel/src/Display/DisplayTypes.h
                    ../../Panel/src/Display/InputWindow.cpp
                    ../../Panel/src/Display/InputWindow.h
                    ../../Panel/src/Display/InputWindowStruct.cpp
                    ../../Panel/src/Display/Painter.cpp
                    ../../Panel/src/Display/Painter.h
                    ../../Panel/src/Display/Symbols.h
                    ../../Panel/src/Display/Text.cpp
                    ../../Panel/src/Display/Text.h
                    ../../Panel/src/Hardware/Controls.cpp
                    ../../Panel/src/Hardware/Controls.h
                    ../../Panel/src/Hardware/CPU.h
                    ../../Panel/src/Hardware/LTDC.h
                    ../../Panel/src/Hardware/stm429.h
                    ../../Panel/src/Hardware/Timer.cpp
                    ../../Panel/src/Hardware/Timer.h
                    ../../Panel/src/Hardware/Timer4XX.h
                    ../../Panel/src/Hardware/VCP.cpp
                    ../../Panel/src/Hardware/VCP.h
                    ../../Panel/src/Interface/HandlersPanel.cpp
                    ../../Panel/src/Interface/HandlersPanel.h
                    ../../Panel/src/Interface/InterfacePanel.cpp
                    ../../Panel/src/Interface/InterfacePanel.h
                    ../../Panel/src/libs/CMSIS/startup_stm32f429xx.s
                                src/GUI/*
                                src/Display/Painter_win.cpp
                                src/Hardware/CPU_win.cpp
                                src/Hardware/CPU_Keyboard_win.cpp
                                src/Hardware/LTDC_win.cpp
                                src/Hardware/Timer4XX_win.cpp
                                src/Hardware/Modules/EEPROM_win.cpp
                                src/Interface/Transceiver_429_win.cpp
                    ../../Panel/src/Display/font/*
                    ../../Panel/src/FDrive/*
                    ../../Panel/src/FrequencyMeter/*
                    ../../Panel/src/Generator/*
                    ../../Panel/src/libs/HAL/*
                    ../../Panel/src/Menu/*
                    ../../Panel/src/Menu/Pages/*
                    ../../Panel/src/Menu/Pages/Addition/*
                    ../../Panel/src/Menu/Pages/Include/*
                    ../../Panel/src/Menu/Pages/PageSignals/*
                    ../../Panel/src/SCPI/*
                    ../../Panel/src/Settings/*
                    ../../Panel/src/Utils/*
                    )

add_executable(G6-49 ${SOURCES})

target_link_libraries(G6-49 ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(G6-49 ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(G6-49 WinMM.lib)
target_link_libraries(G6-49 version.lib)
target_link_libraries(G6-49 Imm32.Lib)
target_link_libraries(G6-49 Setupapi.lib)
