cmake_minimum_required(VERSION 3.15)

project (Editor)

include_directories(src
                    ../../../../ThirdParty/wxWidgets/include
                    ../../../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud
                    ../../../../ThirdParty/SDL2/SDL2-2.0.10/include)
                    
add_definitions(-DWXUSINGDLL -DUSE_SDL2)

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
