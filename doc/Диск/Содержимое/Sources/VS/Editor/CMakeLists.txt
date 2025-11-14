project (Editor)

include_directories(src
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DWXUSINGDLL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MTd")

add_compile_options(/wd4355)    # 'this': used in base member initializer list
add_compile_options(/wd4514)    # 'xxx': unreferenced inline function has been removed
add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

add_compile_options(/wd5219)    # Лоджное срабатываение на 16.7.2


file(GLOB DOC            doc/*)
file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB CONTROLS       src/Controls/*)
file(GLOB DIALOGS        src/Dialogs/*)
file(GLOB EDITOR         src/Editor/*.h src/Editor/*.cpp)
file(GLOB EDITOR_PAINTER src/Editor/Painter/*)
file(GLOB UTILS          src/Utils/*)
file(GLOB WINDOWS        src/Windows/*)

source_group(src             FILES ${SRC})
source_group(doc             FILES ${DOC})
source_group(Controls        FILES ${CONTROLS})
source_group(Dialogs         FILES ${DIALOGS})
source_group(Editor          FILES ${EDITOR})
source_group(Editor\\Painter FILES ${EDITOR_PAINTER})
source_group(Utils           FILES ${UTILS})
source_group(Windows         FILES ${WINDOWS})

add_executable(Editor ${DOC} ${SRC} ${CONTROLS} ${DIALOGS} ${EDITOR} ${EDITOR_PAINTER} ${UTILS} ${WINDOWS})

target_link_libraries(Editor ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(Editor ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(Editor WinMM.lib)
target_link_libraries(Editor version.lib)
target_link_libraries(Editor Imm32.Lib)
target_link_libraries(Editor Setupapi.lib)
