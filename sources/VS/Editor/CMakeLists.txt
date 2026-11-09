# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Editor)

set(CMAKE_UNITY_BUILD OFF)

include_directories(
    src
    ../../generated/ThirdParty/lib/vc_lib
    ../../generated/ThirdParty/lib/vc_lib/mswu
    ../../ThirdParty/wxWidgets/include
)

link_directories(../../generated/ThirdParty/lib/vc_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4355    # 'this': used in base member initializer list
    /wd4514    # 'xxx': unreferenced inline function has been removed
    /wd4625    # copy constructor was implicitly defined as deleted
    /wd4626    # assignment operator was implicitly defined as deleted
    /wd4668
    /wd4820    # 'bytes' bytes padding added after construct 'member_name'
    /wd4865    # the underlying type of '<unnamed-enum-DISPLAYCONFIG_OUTPUT_TECHNOLOGY_OTHER>' will change when '/Zc:enumTypes' is specified on the command line
    /wd5026    # move constructor was implicitly defined as deleted
    /wd5027    # move assignment operator was implicitly defined as deleted
    /wd5039
    /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    /wd5219    # Лоджное срабатываение на 16.7.2
    /wd5267
)

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

add_executable(${PROJECT_NAME}
    ${DOC}
    ${SRC}
    ${CONTROLS}
    ${DIALOGS}
    ${EDITOR}
    ${EDITOR_PAINTER}
    ${UTILS}
    ${WINDOWS}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
    wxbase33u
    wxmsw33u_core
    wxzlib
    wxpng
    WinMM.lib
    version.lib
    Imm32.Lib
    Setupapi.lib
    comctl32.lib
    Rpcrt4.lib
)

