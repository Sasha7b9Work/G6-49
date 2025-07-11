# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Editor)

set(CMAKE_UNITY_BUILD OFF)

include_directories(src
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DWXUSINGDLL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MTd")

# Older cmake versions don't support string(APPEND ...) provide a workaround
macro(wx_string_append var str)
    set(${var} ${${var}}${str})
endmacro()

if(BUILD_USE_STATIC_RUNTIME)
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
endif()

add_compile_options(/wd4355)    # 'this': used in base member initializer list
add_compile_options(/wd4514)    # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4668)
add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)    # ������� ������������� �� 16.7.2
add_compile_options(/wd5267)


file(GLOB DOC            doc/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB CONTROLS       src/Controls/*)
file(GLOB DIALOGS        src/Dialogs/*)
file(GLOB EDITOR         src/Editor/*.h src/Editor/*.cpp)
file(GLOB EDITOR_PAINTER src/Editor/Painter/*)
file(GLOB UTILS          src/Utils/*)
file(GLOB WINDOWS        src/Windows/*)
file(GLOB RESOURCES      resources/*.rc resources/*.ico resources/*.png resources/*.bmp)

set_source_files_properties(resources/*.ico resources/*.png resources/*.bmp resources/*.rc PROPERTIES LANGUAGE RC)

source_group(src             FILES ${SRC})
source_group(doc             FILES ${DOC})
source_group(Controls        FILES ${CONTROLS})
source_group(Dialogs         FILES ${DIALOGS})
source_group(Editor          FILES ${EDITOR})
source_group(Editor\\Painter FILES ${EDITOR_PAINTER})
source_group(Utils           FILES ${UTILS})
source_group(Windows         FILES ${WINDOWS})
source_group(Resources       FILES ${RESOURCES})

add_executable(${PROJECT_NAME} ${DOC}
                               ${SRC}
                               ${CONTROLS}
                               ${DIALOGS}
                               ${EDITOR}
                               ${EDITOR_PAINTER}
                               ${UTILS}
                               ${WINDOWS}
                               ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase32ud)
target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw32ud_core)

target_link_libraries(${PROJECT_NAME} WinMM.lib)
target_link_libraries(${PROJECT_NAME} version.lib)
target_link_libraries(${PROJECT_NAME} Imm32.Lib)
target_link_libraries(${PROJECT_NAME} Setupapi.lib)
