cmake_minimum_required(VERSION 2.9)

# Set CMake policy required by UrhoCommon module
if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
#    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
#    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
#    # Honor the visibility properties for SHARED target types only
#    cmake_policy (SET CMP0063 OLD)
endif ()

# Set project name
project (G6-49_GUI)

set (TARGET_NAME G6-49_GUI)

add_subdirectory(Panel)
add_subdirectory(Editor)
