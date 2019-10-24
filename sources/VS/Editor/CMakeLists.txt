cmake_minimum_required(VERSION 3.15)

project (Editor)

include_directories(src
                    ../../../../ThirdParty/wxWidgets/include
                    ../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud
                    ../../../../ThirdParty/SDL2/SDL2-2.0.10/include)
                    
add_definitions(-DWXUSINGDLL -DUSE_SDL2)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")


add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified


source_group(src REGULAR_EXPRESSION src/*)

file(GLOB SOURCES src/*)

add_executable(Editor ${SOURCES})

target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(Editor ../../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)
target_link_libraries(Editor ../../../../../ThirdParty/SDL2/SDL2-2.0.10/generated/Debug/SDL2-staticd)

target_link_libraries(Editor WinMM.lib)
target_link_libraries(Editor version.lib)
target_link_libraries(Editor Imm32.Lib)
target_link_libraries(Editor Setupapi.lib)
