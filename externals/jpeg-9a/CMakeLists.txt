project(libjpeg C)

set(DEPENDS zlib)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # Disable warnings on deprecated items
    add_definitions(/wd4996)
    add_definitions(/wd4267)

endif()

build_static_library()

set(JPEG_INCLUDE_DIR ${libjpeg_INCLUDES} CACHE PATH "JPEG library includes" FORCE)
set(JPEG_LIBRARY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libjpeg.${LIB_SUFFIX}" CACHE PATH "JPEG library" FORCE)
