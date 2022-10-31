include(ExternalProject)


set(mrViewer_ARGS
  ${TLRENDER_EXTERNAL_ARGS}
  -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
  -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DTLRENDER_MMAP=${TLRENDER_MMAP}
  -DTLRENDER_PYTHON=${TLRENDER_PYTHON}
  -DTLRENDER_OCIO=${TLRENDER_OCIO}
  -DTLRENDER_AUDIO=${TLRENDER_AUDIO}
  -DTLRENDER_FREETYPE=${TLRENDER_FREETYPE}
  -DTLRENDER_JPEG=${TLRENDER_JPEG}
  -DTLRENDER_TIFF=${TLRENDER_TIFF}
  -DTLRENDER_PNG=${TLRENDER_PNG}
  -DTLRENDER_EXR=${TLRENDER_EXR}
  -DTLRENDER_FFMPEG=${TLRENDER_FFMPEG}
  -DTLRENDER_GL=ON
  -DTLRENDER_BMD=${TLRENDER_BUILD_BMD}
  -DTLRENDER_BMD_SDK=${TLRENDER_BMD_SDK}
  )

ExternalProject_Add(
    mrViewer2
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/mrViewer
    DEPENDS tlRender FLTK BOOST
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/mrViewer
    LIST_SEPARATOR |
    CMAKE_ARGS ${mrViewer_ARGS})
