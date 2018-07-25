cmake_minimum_required(VERSION 3.9.0)

project(aurum)

# Target directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_PLUGINS_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Installation destination
set(INSTALL_BIN_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INSTALL_LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(INSTALL_PATCHES_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INSTALL_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)

# Target prefix
set(DYLIB_SUFFIX "so")
set(LIB_SUFFIX "a")

if(MINGW)
    set(DYLIB_SUFFIX "dll")
elseif(MSVC)
    set(DYLIB_SUFFIX "dll")
    set(LIB_SUFFIX "lib")
endif()


# CMake custom scripts
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(VST_PLUGIN_DLL_DEF ${PROJECT_SOURCE_DIR}/cmake/plugin.def)
include(macros)

# Enforce static linking
if(MSVC)
    add_compile_options(/MP)

    set(COMPILER_FLAGS
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
    )

    foreach(flag ${COMPILER_FLAGS})
        string(REPLACE "/MD" "/MT" "${flag}" "${${flag}}")
        string(REPLACE "/Zi" "/ZI" "${flag}" "${${flag}}")
        string(REPLACE "/EHs" "/EHa" "${flag}" "${${flag}}")
    endforeach()
endif()

add_subdirectory(externals)
add_subdirectory(lib)
add_subdirectory(source)
