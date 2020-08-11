project (Editor)

include_directories(src
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DWXUSINGDLL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MTd")


# Older cmake versions don't support string(APPEND ...) provide a workaround
macro(wx_string_append var str)
    set(${var} ${${var}}${str})
endmacro()

set(MSVC_LIB_USE "/MTd")
set(MSVC_LIB_REPLACE "/MDd")

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

add_compile_options(/wd4355)    # 'this': used in base member initializer list
add_compile_options(/wd4514)    # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified


file(GLOB SRC     src/*)
file(GLOB DIALOGS src/Dialogs/*)

source_group(src     FILES ${SRC})
source_group(Dialogs FILES ${DIALOGS})

add_executable(Editor ${SRC} ${DIALOGS})

target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(Editor WinMM.lib)
target_link_libraries(Editor version.lib)
target_link_libraries(Editor Imm32.Lib)
target_link_libraries(Editor Setupapi.lib)
