cmake_minimum_required(VERSION 3.23)



project( mrv2 )

if(APPLE)
    message( STATUS "-------------------------------------------------" )
    message( STATUS "Building for macOS ${CMAKE_OSX_DEPLOYMENT_TARGET}" )
    message( STATUS "-------------------------------------------------" )
endif()

if(TLRENDER_NDI)
    if(NOT EXISTS "${TLRENDER_NDI_SDK}")
	message(FATAL_ERROR "TLRENDER_NDI is ON but TLRENDER_NDI_SDK does not exist!\n${TLRENDER_NDI_SDK}") 
    endif()
endif()


cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0114 NEW)
if( POLICY CMP0135 )
    cmake_policy(SET CMP0135 NEW)
endif()

include( cmake/version.cmake )
include( cmake/options.cmake )
include( cmake/functions.cmake )

if (APPLE)
    message(STATUS "OLD_DYLD_LIBRARY_PATH=$ENV{OLD_DYLD_LIBRARY_PATH}")
    message( STATUS "-------------------------------------------------" )
    message(STATUS "DYLD_LIBRARY_PATH=$ENV{DYLD_LIBRARY_PATH}")
    message( STATUS "-------------------------------------------------" )
elseif(UNIX)
    message(STATUS "OLD_LD_LIBRARY_PATH=$ENV{OLD_LD_LIBRARY_PATH}")
    message( STATUS "-------------------------------------------------" )
    message(STATUS "LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}")
    message( STATUS "-------------------------------------------------" )
else()
    message(STATUS "OLD_PATH=$ENV{OLD_PATH}")
    message( STATUS "-------------------------------------------------" )
    message(STATUS "PATH=$ENV{PATH}")
    message( STATUS "-------------------------------------------------" )
endif()


#
# Fetch the tlRender library as a submodule with git.
#

if(GIT_SUBMODULE)
    find_package(Git REQUIRED)
    
    message(STATUS "Update/init tlRender's submodules (FSeq, etc)")
    execute_process(
	COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
	RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
	message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

# Omit superfluous "Up-to-date" messages.
if(NOT DEFINED CMAKE_INSTALL_MESSAGE)
    set(CMAKE_INSTALL_MESSAGE "LAZY")
endif()

#
# Python cannot be built with asan dependencies
#
set( PYTHON_DEP )
if( BUILD_PYTHON )
    include( cmake/Modules/BuildPython.cmake )
    
    #
    # This is needed for our own tlRender's USD building
    #
    if(UNIX)
	set( Python_EXECUTABLE ${CMAKE_INSTALL_PREFIX}/bin/python${Python_VERSION} )
    else()
	set( Python_EXECUTABLE ${CMAKE_INSTALL_PREFIX}/bin/python.exe )
    endif()
else()
    if (UNIX)
	set(PYTHON_EXECUTABLE python3)
    else()
	set(PYTHON_EXECUTABLE python.exe)
    endif()
endif()


    set(PYTHON_EXECUTABLE ${Python_EXECUTABLE} )

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    # Check for Clang or GCC compiler  (@todo: add Microsoft support)
    if (TLRENDER_ASAN AND
	    ((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR
		(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")))
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
	set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=address")
    endif()
endif()


set( Gettext )
if( BUILD_GETTEXT )
    include( cmake/Modules/BuildGettext.cmake )
elseif(APPLE)
    if (CMAKE_OSX_DEPLOYMENT_TARGET)
	#
	# brew .dylibs are compiled only for the current version of the OS.
	#
	# We distribute libintl dylib compatible with v10.5
	file(COPY precompiled/macOS/v10.5/ DESTINATION ${CMAKE_INSTALL_PREFIX}/
	    FOLLOW_SYMLINK_CHAIN)
    endif()
endif()


set( PYBIND11_DEP )
if( MRV2_PYBIND11 )
    include( cmake/Modules/Buildpybind11.cmake )
endif()

add_subdirectory( tlRender/etc/SuperBuild )

set(FLTK_DEP tlRender )
if( BUILD_FLTK )
    include( cmake/Modules/BuildFLTK.cmake )
endif()

if(BUILD_PYTHON AND TLRENDER_USD)
    add_dependencies( USD ${PYTHON_DEP} )
endif()

set( pyFLTK_DEP )
if(MRV2_PYFLTK AND MRV2_PYBIND11)
    include( cmake/Modules/BuildpyFLTK.cmake )
endif()

# OpenSSL must be built before Python and pyFLTK to use the right path when
# building Python's _ssl module.
if(TLRENDER_NET AND NOT WIN32)
    if (BUILD_PYTHON)
	add_dependencies( Python OpenSSL)
    endif()
endif()


set( POCO_DEP )
if( MRV2_NETWORK )
    include( cmake/Modules/BuildPoco.cmake )
endif()

set( LibHaru_DEP )
if( MRV2_PDF )
    include( cmake/Modules/BuildLibHaru.cmake )
endif()

if( BUILD_MRV2 )
    include( cmake/Modules/Buildmrv2.cmake )
endif()

