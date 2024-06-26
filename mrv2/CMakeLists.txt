cmake_minimum_required(VERSION 3.19)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0114 NEW)


project(mrv2)

# For macOS, finds Frameworks last.  Uses Find*.cmake first.
set(CMAKE_FIND_FRAMEWORK LAST)


include( ../cmake/version.cmake )
include( ../cmake/options.cmake )
include( ../cmake/functions.cmake )

list(PREPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/../tlRender/cmake/Modules
    ${PROJECT_SOURCE_DIR}/../cmake/Modules)





#
# Add compile options
#
if ( UNIX )
    add_compile_options( -fvisibility=hidden )
    if (NOT APPLE)
	link_directories(${CMAKE_INSTALL_PREFIX}/lib64)
    endif()
    link_directories(${CMAKE_INSTALL_PREFIX}/lib)
endif()

find_package(FLTK CONFIG REQUIRED)

if(MSVC)
    set(FLTK_LIBRARIES fltk::fltk-shared )
    set(FLTK_gl_LIBRARY )
else()
    # fltk::images must *NOT* be shared or jpeg library enters in conflict
    # with tlRender's libjpeg-turbo.
    set(FLTK_LIBRARIES fltk::fltk-shared fltk::images )
    set(FLTK_gl_LIBRARY fltk::gl-shared)
endif()

#
# Make sure we use our own recently compiled fluid executable
#
if(MSVC)
    set(FLTK_FLUID_EXECUTABLE ${CMAKE_INSTALL_PREFIX}/bin/fluid-cmd.exe)
else()
    set(FLTK_FLUID_EXECUTABLE ${CMAKE_INSTALL_PREFIX}/bin/fluid)
endif()

message( STATUS "Found FLTK: ${FLTK_INCLUDE_DIR}" )
message( STATUS "FLTK LIBRARIES: ${FLTK_gl_LIBRARY} ${FLTK_LIBRARIES}" )
message( STATUS "FLTK_FLUID_EXECUTABLE: ${FLTK_FLUID_EXECUTABLE}" )


find_package(Intl)
find_package(tlRender REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(glfw3 REQUIRED)
find_package(minizip REQUIRED)

if( MRV2_NETWORK )
    find_package(Poco REQUIRED Net Foundation)
    add_definitions( -DMRV2_NETWORK )
endif()

set(LibHaru )
if( MRV2_PDF )
    find_library( LibHaru NAMES hpdf )
    if(UNIX AND NOT APPLE)
	list(APPEND LibHaru m)
    endif()
    add_definitions( -DMRV2_PDF )
endif()

if(TLRENDER_BMD)
    add_definitions(-DTLRENDER_BMD)
endif()

if(TLRENDER_EXR)
    add_definitions(-DTLRENDER_EXR)
endif()

if(TLRENDER_FFMPEG)
    add_definitions(-DTLRENDER_FFMPEG)
endif()

if(TLRENDER_GL)
    add_definitions(-DTLRENDER_GL)
endif()

if(TLRENDER_JPEG)
    add_definitions(-DTLRENDER_JPEG)
endif()

if(TLRENDER_NDI)
    add_definitions(-DTLRENDER_NDI)
endif()

if(TLRENDER_NET)
    add_definitions(-DTLRENDER_NET)
endif()

if(TLRENDER_OCIO)
    add_definitions(-DTLRENDER_OCIO)
endif()

if(TLRENDER_PNG)
    add_definitions(-DTLRENDER_PNG)
endif()

if(TLRENDER_RAW)
    find_package(Jasper)
    if (JASPER_FOUND)
        add_definitions(-DTLRENDER_RAW)
    endif()
endif()

if(TLRENDER_STB)
    add_definitions(-DTLRENDER_STB)
endif()

if(TLRENDER_TIFF)
    add_definitions(-DTLRENDER_TIFF)
endif()

if(TLRENDER_USD)
    find_package(pxr)
    if(pxr_FOUND)
        add_definitions(-DTLRENDER_USD)
    endif()
endif()

if(TLRENDER_X264)
    add_definitions(-DTLRENDER_X264)
endif()


if(MRV2_PYBIND11)
    add_definitions(-DMRV2_PYBIND11)
    find_package(pybind11 REQUIRED)
endif()

if(MRV2_PYFLTK)
    add_definitions(-DMRV2_PYFLTK)
endif()

# Omit superfluous "Up-to-date" messages.
if(NOT DEFINED CMAKE_INSTALL_MESSAGE)
    set(CMAKE_INSTALL_MESSAGE "LAZY")
endif()

#
# Add some defines
#
add_definitions(
    -D NOMINMAX
    -DMRV2_VERSION="${mrv2_VERSION}"
    -DTLRENDER_VERSION="${tlRender_VERSION}"
)


# Directory of generated .cxx/.h built from .fl files
set( mrv2_FLUID_INCLUDE_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/lib/mrvWidgets
    )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DTLRENDER_API_GL_4_1_Debug )
    set(tlRender_glad "${CMAKE_CURRENT_SOURCE_DIR}/../tlRender/deps/glad_GL_4_1_Debug")
else()
   set(tlRender_glad "${CMAKE_CURRENT_SOURCE_DIR}/../tlRender/deps/glad_GL_4_1")
endif()

file(GLOB mrv2_BOOST_DIRS "${CMAKE_INSTALL_PREFIX}/include/boost*")

set( mrv2_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/lib   # for mrv2 includes (search here first)
    ${mrv2_FLUID_INCLUDE_DIR}         # for mrv2 .fl converted files next
    ${tlRender_glad}
    ${CMAKE_PREFIX_PATH}/include/OpenEXR
    ${CMAKE_PREFIX_PATH}/include/Imath
    ${CMAKE_PREFIX_PATH}/include/opentimelineio/deps
    ${mrv2_BOOST_DIRS}
    ${CMAKE_PREFIX_PATH}/include/tlRender
    ${CMAKE_PREFIX_PATH}/include
    ${tlRender_INCLUDE_DIRS}
    # We need to include all these non-public tlRender directories
    # as we get the version information from them as well as some variables
    # and constants.
    ${TLRENDER_NDI_SDK}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../tlRender/deps
    ${CMAKE_CURRENT_SOURCE_DIR}/../tlRender/lib
    # For icons on the mrv2/icons directory.
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Intl_INCLUDE_DIRS}
)

include_directories( ${mrv2_INCLUDE_DIRS} )

#
# Install mrv2's auxiliary application directories
#
install( DIRECTORY colors fonts icons ocio presets share
    DESTINATION .
    COMPONENT applications )

#
# Install update-mrv2 plugin
#
install( FILES python/plug-ins/update-mrv2.py
    DESTINATION python/plug-ins
    COMPONENT applications )

#
# Install mrv2's documentation
#
install( DIRECTORY docs
    DESTINATION .
    COMPONENT documentation )

#
# Install mrv2's python demos
#
install( DIRECTORY python/demos
    DESTINATION python
    COMPONENT python_demos )

install( DIRECTORY python/plug-ins/demos
    DESTINATION python/plug-ins
    COMPONENT python_demos )

#
# These two variables will contain the sources for .po tranalation.
# PO_SOURCES will contain relative paths while PO_ABS_SOURCES will conttain
# the absolute paths.
#
set( PO_SOURCES )
set( PO_ABS_SOURCES )

#
# Add auxiliary libraries
#
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/lib )

#
# Add main source
#
add_subdirectory( src )

#
# Add the packaging logic
#
include( ../cmake/packaging.cmake )
