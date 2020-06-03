cmake_minimum_required(VERSION 3.0)

project (G6-49)

include_directories(src
                    ../../Panel/src
                    ../../Panel/src/libs/HAL
                    ../../Panel/src/Hardware
                    ../..
                    ../../common/_VS_GUI
                    ../../common/_VS_GUI/CMSIS
                    ../../common/_VS_GUI/HAL
                    ../../Panel/src/libs/HAL
                    ../../../../ThirdParty/wxWidgets/include
                    ../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)

add_definitions(-DSTM32F437xx -DWXUSINGDLL -DUSE_SDL2 -DPANEL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

# Older cmake versions don't support string(APPEND ...) provide a workaround
macro(wx_string_append var str)
    set(${var} ${${var}}${str})
endmacro()

set(MSVC_LIB_USE "/MDd")
set(MSVC_LIB_REPLACE "/MTd")
if(CONF_RELEASE)
    set(MSVC_LIB_USE "/MTd")
    set(MSVC_LIB_REPLACE "/MDd")
endif()
# Set MSVC runtime flags for all configurations
foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
    set(c_flag_var CMAKE_C_FLAGS)
    set(cxx_flag_var CMAKE_CXX_FLAGS)
    if(cfg)
        string(TOUPPER ${cfg} cfg_upper)
        wx_string_append(c_flag_var "_${cfg_upper}")
        wx_string_append(cxx_flag_var "_${cfg_upper}")
    endif()
    if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
        string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
        set(${c_flag_var} ${${c_flag_var}} CACHE STRING
        "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
    endif()
    if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
        string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
        set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
        "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE)
    endif()
endforeach()


add_compile_options(/wd4211)    # nonstandard extension used: redefined extern to static
add_compile_options(/wd4355)    # used in base member initializer list
add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'xxx': default constructor was implicitly defined as deleted
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
add_compile_options(/wd26812)   # The enum type 'xxx' in unscoped. Prefer 'enum class' over 'enum'

source_group(common                   FILES              ../../common/Command.h
                                                         ../../common/Command.cpp
                                                         ../../common/Common.h
                                                         ../../common/Common.cpp
                                                         ../../common/Common_pan.cpp
                                                         ../../common/CommonTypes.h
                                                         ../../common/Messages.h
                                                         ../../common/Messages.cpp
                                                         ../../common/Transceiver.h)
source_group(common\\_VS\\HAL         FILES              ../../common/_VS_GUI/HAL/stm32f4xx_hal.cpp)
source_group(src                      FILES              ../../Panel/src/defines.h
                                                         ../../Panel/src/Log.cpp
                                                         ../../Panel/src/Log.h
                                                         ../../Panel/src/stdafx.cpp
                                                         ../../Panel/src/stdafx.h
                                                         ../../Panel/src/structs.h)
source_group(Display                  FILES              ../../Panel/src/Display/Colors.h
                                                         ../../Panel/src/Display/Colors.cpp
                                                         ../../Panel/src/Display/Console.h
                                                         ../../Panel/src/Display/Console.cpp
                                                         ../../Panel/src/Display/Display.h
                                                         ../../Panel/src/Display/Display.cpp
                                                         ../../Panel/src/Display/DisplayTypes.h
                                                         ../../Panel/src/Display/Painter.h
                                                         ../../Panel/src/Display/Painter.cpp
                                                                     src/Display/Painter_win.cpp
                                                         ../../Panel/src/Display/Symbols.h
                                                         ../../Panel/src/Display/Text.h
                                                         ../../Panel/src/Display/Text.cpp
                                                         ../../Panel/src/Display/Tuner.h
                                                         ../../Panel/src/Display/Tuner.cpp
                                                         ../../Panel/src/Display/Viewer.h
                                                         ../../Panel/src/Display/Viewer.cpp
                                                         ../../Panel/src/Display/WaveGraphics.h
                                                         ../../Panel/src/Display/WaveGraphics.cpp)
source_group(Hardware                 FILES                          src/Hardware/Beeper_win.cpp
                                                         ../../Panel/src/Hardware/CPU.h
                                                         ../../Panel/src/Hardware/Timer.h
                                                         ../../Panel/src/Hardware/Timer.cpp
                                                         ../../Panel/src/Hardware/VCP_p.h
                                                                     src/Hardware/CPU_win.cpp
                                                                     src/Hardware/LTDC_win.cpp
                                                                     src/Hardware/VCP_p_win.cpp)
source_group(Hardware\\HAL            FILES              ../../Panel/src/Hardware/HAL/HAL_TIM.cpp
                                                                     src/Hardware/HAL/HAL_EEPROM_win.cpp
                                                                     src/Hardware/HAL/HAL_PIO_win.cpp
                                                                     src/Hardware/HAL/HAL_TIM2_win.cpp
                                                                     src/Hardware/HAL/HAL_TIM3_win.cpp
                                                                     src/Hardware/HAL/HAL_TIM4_win.cpp)
source_group(Keyboard                 FILES              ../../Panel/src/Keyboard/Controls.h
                                                         ../../Panel/src/Keyboard/Controls.cpp
                                                         ../../Panel/src/Keyboard/Keyboard.h
                                                         ../../Panel/src/Keyboard/Keyboard.cpp
                                                                     src/Keyboard/Keyboard_win.h)
source_group(Interface                FILES              ../../Panel/src/Interface/Handlers_p.cpp
                                                         ../../Panel/src/Interface/Handlers_p.h
                                                         ../../Panel/src/Interface/Interface_p.cpp
                                                         ../../Panel/src/Interface/Interface_p.h
                                                         ../../Panel/src/Interface/Messages_p.h
                                                         ../../Panel/src/Interface/Messages_p.cpp
                                                                     src/Interface/Transceiver_p_win.cpp)
source_group(Display\\Font            REGULAR_EXPRESSION ../Panel/src/Display/Font/*)
source_group(FDrive                   REGULAR_EXPRESSION ../Panel/src/FDrive/*)
source_group(FreqMeter                REGULAR_EXPRESSION ../Panel/src/FreqMeter/*)
source_group(Generator                REGULAR_EXPRESSION ../Panel/src/Generator/*)
source_group(GUI                      REGULAR_EXPRESSION src/GUI/*)
source_group(libs\\CMSIS              REGULAR_EXPRESSION ../Panel/src/libs/CMSIS/*)
source_group(libs\\HAL                REGULAR_EXPRESSION ../Panel/src/libs/HAL/*)
source_group(Menu                     REGULAR_EXPRESSION ../Panel/src/Menu/*)
source_group(Menu\\Pages              REGULAR_EXPRESSION ../Panel/src/Menu/Pages/*)
source_group(Menu\\Pages\\Addition    REGULAR_EXPRESSION ../Panel/src/Menu/Pages/Addition/*)
source_group(Menu\\Pages\\PageDebug   REGULAR_EXPRESSION ../Panel/src/Menu/Pages/PageDebug/*)
source_group(SCPI                     REGULAR_EXPRESSION ../Panel/src/SCPI/*)
source_group(Settings                 REGULAR_EXPRESSION ../Panel/src/Settings/*)
source_group(Utils                    REGULAR_EXPRESSION ../Panel/src/Utils/*)

file(GLOB SOURCES   ../../common/Command.h
                    ../../common/Command.cpp
                    ../../common/Common.h
                    ../../common/Common.cpp
                    ../../common/Common_pan.cpp
                    ../../common/CommonTypes.h
                    ../../common/Messages.h
                    ../../common/Messages.cpp
                    ../../common/Messages_p.h
                    ../../common/Messages_p.cpp
                    ../../common/Transceiver.h
                    
                    ../../common/_VS_GUI/HAL/stm32f4xx_hal.cpp
                    
                    ../../Panel/src/defines.h
                    ../../Panel/src/Log.cpp
                    ../../Panel/src/Log.h
                    ../../Panel/src/stdafx.cpp
                    ../../Panel/src/stdafx.h
                    ../../Panel/src/structs.h
                    
                    ../../Panel/src/Display/Colors.h
                    ../../Panel/src/Display/Colors.cpp
                    ../../Panel/src/Display/Console.h
                    ../../Panel/src/Display/Console.cpp
                    ../../Panel/src/Display/Display.h
                    ../../Panel/src/Display/Display.cpp
                    ../../Panel/src/Display/DisplayTypes.h
                    ../../Panel/src/Display/Painter.h
                    ../../Panel/src/Display/Painter.cpp
                                src/Display/Painter_win.cpp
                    ../../Panel/src/Display/Symbols.h
                    ../../Panel/src/Display/Text.h
                    ../../Panel/src/Display/Text.cpp
                    ../../Panel/src/Display/Tuner.h
                    ../../Panel/src/Display/Tuner.cpp
                    ../../Panel/src/Display/Viewer.h
                    ../../Panel/src/Display/Viewer.cpp
                    ../../Panel/src/Display/WaveGraphics.h
                    ../../Panel/src/Display/WaveGraphics.cpp
                    
                    ../../Panel/src/Hardware/Controls.h
                    ../../Panel/src/Hardware/Controls.cpp
                    ../../Panel/src/Hardware/CPU.h
                    ../../Panel/src/Hardware/Keyboard.h
                    ../../Panel/src/Hardware/Keyboard.cpp
                    ../../Panel/src/Hardware/Timer.h
                    ../../Panel/src/Hardware/Timer.cpp
                    ../../Panel/src/Hardware/VCP_p.h
                                src/Hardware/Beeper_win.cpp
                                src/Hardware/CPU_win.cpp
                                src/Hardware/Keyboard_win.h
                                src/Hardware/LTDC_win.cpp
                                src/Hardware/VCP_p_win.cpp

                    ../../Panel/src/Hardware/HAL/HAL_TIM.cpp
                                src/Hardware/HAL/HAL_EEPROM_win.cpp
                                src/Hardware/HAL/HAL_PIO_win.cpp
                                src/Hardware/HAL/HAL_TIM2_win.cpp
                                src/Hardware/HAL/HAL_TIM3_win.cpp
                                src/Hardware/HAL/HAL_TIM4_win.cpp
                                
                                
                    ../../Panel/src/Keyboard/Controls.h
                    ../../Panel/src/Keyboard/Controls.cpp
                    ../../Panel/src/Keyboard/Keyboard.h
                    ../../Panel/src/Keyboard/Keyboard.cpp
                                src/Keyboard/Keyboard_win.h
                    
                    ../../Panel/src/Interface/Handlers_p.cpp
                    ../../Panel/src/Interface/Handlers_p.h
                    ../../Panel/src/Interface/Interface_p.cpp
                    ../../Panel/src/Interface/Interface_p.h
                    ../../Panel/src/Interface/Messages_p.h
                    ../../Panel/src/Interface/Messages_p.cpp
                                src/Interface/Transceiver_p_win.cpp
                                
                    ../../Panel/src/libs/CMSIS/startup_stm32f429xx.s
                                src/GUI/*
                                src/Display/Painter_win.cpp

                    ../../Panel/src/Display/Font/*
                    ../../Panel/src/FDrive/*
                    ../../Panel/src/FreqMeter/*
                    ../../Panel/src/Generator/*
                    ../../Panel/src/libs/HAL/*
                    ../../Panel/src/Menu/*
                    ../../Panel/src/Menu/Pages/*
                    ../../Panel/src/Menu/Pages/Addition/*
                    ../../Panel/src/Menu/Pages/PageDebug/*
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
