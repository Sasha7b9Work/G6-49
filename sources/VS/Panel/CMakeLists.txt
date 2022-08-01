# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (G6-49)

set(CMAKE_UNITY_BUILD OFF)

include_directories(src
                    ../../Panel/src
                    ../../Panel/src/Hardware
                    ../..
                    ../../common/_VS_GUI
                    ../../common/_VS_GUI/CMSIS
                    ../../common/_VS_GUI/HAL
                    ../../Panel/src/Hardware/HAL
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)

add_definitions(-DSTM32F437xx -DWXUSINGDLL -DUSE_SDL2 -DGUI -DPANEL)

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


add_compile_options(/wd4103)
add_compile_options(/wd4211)    # nonstandard extension used: redefined extern to static
add_compile_options(/wd4355)    # used in base member initializer list
add_compile_options(/wd4514)    # unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'xxx': default constructor was implicitly defined as deleted
add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4668)
add_compile_options(/wd4710)    # function not inlined
add_compile_options(/wd4774)    # format string expected in argument 3 is not a string literal
add_compile_options(/wd4820)    # 'N' bytes padding added after data member
add_compile_options(/wd4946)    # reinterpret_cast used between related classes: 'Page' and 'Item'
add_compile_options(/wd4996)    # function or variable may be unsafe
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5246)
add_compile_options(/wd26812)   # The enum type 'xxx' in unscoped. Prefer 'enum class' over 'enum'

file(GLOB COMMON                 ../../common/Command.*
                                 ../../common/Common.*
                                 ../../common/Common_pan.cpp
                                 ../../common/CommonTypes.h
                                 ../../common/Messages.*
                                 ../../common/Transceiver.h)
file(GLOB COMMON_VS_HAL          ../../common/_VS_GUI/HAL/stm32f4xx_hal.cpp)
file(GLOB SRC                    ../../Panel/src/defines.h
                                 ../../Panel/src/Log.*
                                 ../../Panel/src/stdafx.*
                                 ../../Panel/src/structs.h)
file(GLOB DISPLAY                ../../Panel/src/Display/Colors.*
                                 ../../Panel/src/Display/Console.*
                                 ../../Panel/src/Display/Display.*
                                 ../../Panel/src/Display/DisplayTypes.h
                                 ../../Panel/src/Display/Painter.*
                                             src/Display/Painter_win.cpp
                                 ../../Panel/src/Display/Symbols.h
                                 ../../Panel/src/Display/Text.*
                                 ../../Panel/src/Display/Tuner.*
                                 ../../Panel/src/Display/Viewer.*
                                 ../../Panel/src/Display/WaveGraphics.*)
file(GLOB HARDWARE                           src/Hardware/Beeper_win.cpp
                                 ../../Panel/src/Hardware/CPU.h
                                 ../../Panel/src/Hardware/Timer.*
                                 ../../Panel/src/Hardware/VCP_p.h
                                             src/Hardware/CPU_win.cpp
                                             src/Hardware/LTDC_win.cpp
                                             src/Hardware/VCP_p_win.cpp)
file(GLOB HARDWARE_HAL           ../../Panel/src/Hardware/HAL/HAL_TIM.cpp
                                             src/Hardware/HAL/HAL_EEPROM_win.cpp
                                             src/Hardware/HAL/HAL_PIO_win.cpp
                                             src/Hardware/HAL/HAL_TIM2_win.cpp
                                             src/Hardware/HAL/HAL_TIM3_win.cpp
                                             src/Hardware/HAL/HAL_TIM4_win.cpp)
file(GLOB KEYBOARD               ../../Panel/src/Keyboard/Controls.*
                                 ../../Panel/src/Keyboard/Keyboard.*
                                             src/Keyboard/Keyboard_win.h)
file(GLOB INTERFACE              ../../Panel/src/Interface/Handlers_p.*
                                 ../../Panel/src/Interface/Interface_p.*
                                 ../../Panel/src/Interface/Messages_p.*
                                             src/Interface/Transceiver_p_win.cpp)
file(GLOB DISPLAY_FONT           ../../Panel/src/Display/Font/*)
file(GLOB FDRIVE                 ../../Panel/src/FDrive/*)
file(GLOB FREQMETER              ../../Panel/src/FreqMeter/*)
file(GLOB GENERATOR              ../../Panel/src/Generator/*)
file(GLOB GUI                                src/GUI/*)
file(GLOB LIBS_CMSIS             ../../Panel/src/libs/CMSIS/*)
file(GLOB LIBS_HAL               ../../Panel/src/libs/HAL/*)
file(GLOB MENU                   ../../Panel/src/Menu/*.h ../../Panel/src/Menu/*.cpp)
file(GLOB MENU_PAGES             ../../Panel/src/Menu/Pages/*.h ../../Panel/src/Menu/Pages/*.cpp)
file(GLOB MENU_PAGES_ADDITION    ../../Panel/src/Menu/Pages/Addition/*)
file(GLOB MENU_PAGES_PAGEDEBUG   ../../Panel/src/Menu/Pages/PageDebug/*)
file(GLOB SCPI                   ../../Panel/src/SCPI/*)
file(GLOB SETTINGS               ../../Panel/src/Settings/*)
file(GLOB SIGNALS                ../../Panel/src/Signals/*)
file(GLOB TESTS                  ../../Panel/src/Tests/*)
file(GLOB UTILS                  ../../Panel/src/Utils/*)

source_group(common                 FILES ${COMMON})
source_group(common\\_VS\\HAL       FILES ${COMMON_VS_HAL})
source_group(src                    FILES ${SRC})
source_group(Display                FILES ${DISPLAY})
source_group(Display\\Font          FILES ${DISPLAY_FONT})
source_group(FDrive                 FILES ${FDRIVE})
source_group(FreqMeter              FILES ${FREQMETER})
source_group(Generator              FILES ${GENERATOR})
source_group(GUI                    FILES ${GUI})
source_group(Hardware               FILES ${HARDWARE})
source_group(Hardware\\HAL          FILES ${HARDWARE_HAL})
source_group(Interface              FILES ${INTERFACE})
source_group(Keyboard               FILES ${KEYBOARD})
source_group(libs\\CMSIS            FILES ${LIBS_CMSIS})
source_group(libs\\HAL              FILES ${LIBS_HAL})
source_group(Menu                   FILES ${MENU})
source_group(Menu\\Pages            FILES ${MENU_PAGES})
source_group(Menu\\Pages\\Addition  FILES ${MENU_PAGES_ADDITION})
source_group(Menu\\Pages\\PageDebug FILES ${MENU_PAGES_PAGEDEBUG})
source_group(SCPI                   FILES ${SCPI})
source_group(Settings               FILES ${SETTINGS})
source_group(Signals                FILES ${SIGNALS})
source_group(Tests                  FILES ${TESTS})
source_group(Utils                  FILES ${UTILS})

add_executable(G6-49 ${COMMON} ${COMMON_VS_HAL} ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${FDRIVE} ${FREQMETER} ${GENERATOR}
                     ${GUI} ${HARDWARE} ${HARDWARE_HAL} ${INTERFACE} ${KEYBOARD} ${LIBS_CMSIS} ${LIBS_HAL} ${MENU} ${MENU_PAGES}
                     ${MENU_PAGES_ADDITION} ${MENU_PAGES_PAGEDEBUG} ${SCPI} ${TESTS} ${SETTINGS} ${SIGNALS} ${UTILS})

target_link_libraries(G6-49 ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(G6-49 ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(G6-49 WinMM.lib)
target_link_libraries(G6-49 version.lib)
target_link_libraries(G6-49 Imm32.Lib)
target_link_libraries(G6-49 Setupapi.lib)
