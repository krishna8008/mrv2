# SPDX-License-Identifier: BSD-3-Clause
# mrv2 
# Copyright Contributors to the mrv2 Project. All rights reserved.

include( ExternalProject )

# Stable TAG
set( NNG_REPOSITORY "https://github.com/nanomsg/nng.git" )
set( NNG_TAG v1.5.2 )

ExternalProject_Add(
    NNG
    GIT_REPOSITORY ${NNG_REPOSITORY}
    GIT_TAG ${NNG_TAG}
    GIT_PROGRESS 1
    PATCH_COMMAND ${patch_cmd}
    CMAKE_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHIECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
    -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_INSTALL_MESSAGE=${CMAKE_INSTALL_MESSAGE}
    )
