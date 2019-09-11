cmake_minimum_required(VERSION 3.15)

project (Editor)

include_directories(src
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud
                    ../../ThirdParty/SDL2/SDL2-2.0.9/include)
                    
add_definitions(-DWXUSINGDLL -DUSE_SDL2)

source_group(src REGULAR_EXPRESSION src/*)

file(GLOB SOURCES src/*)

add_executable(Editor ${SOURCES})