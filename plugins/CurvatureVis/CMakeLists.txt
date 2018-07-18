cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project( Cliniface_Plugin_CurvatureVis)

set(WITH_FACETOOLS TRUE)
include( "cmake/FindLibs.cmake")

set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src")

include_directories( ${INCLUDE_DIR})
include_directories( ${PROJECT_BINARY_DIR})

set( SOURCES
    "${SRC_DIR}/CurvatureVis.cpp"
    )

set( QOBJECTS
    "${INCLUDE_DIR}/CurvatureVis.h"
    )

set( RESOURCE_FILE "cvis_icons.qrc")

qt5_wrap_cpp( QOBJECT_MOCS ${QOBJECTS})
qt5_add_resources( RESOURCE_FILE ${RESOURCE_FILE})

add_library( ${PROJECT_NAME} ${SOURCES} ${QOBJECTS} ${QOBJECT_MOCS} ${RESOURCE_FILE})
include(GenerateExportHeader)
generate_export_header( ${PROJECT_NAME}
                        BASE_NAME ${PROJECT_NAME}
                        EXPORT_MACRO_NAME ${PROJECT_NAME}_EXPORT
                        EXPORT_FILE_NAME ${PROJECT_NAME}_Export.h
                        STATIC_DEFINE ${PROJECT_NAME}_BUILT_AS_STATIC)

include( "cmake/LinkTargets.cmake")

# Rename the library if compiled using MSVC to append the MSVC version so as to avoid DLL HELL.
if(WIN32)
    include( "cmake/Macros.cmake")
    get_msvc_version( _msvcv)
    set_target_properties( ${PROJECT_NAME} PROPERTIES SUFFIX "-vc${_msvcv}0.dll")
    set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORT_SUFFIX "-vc${_msvcv}0.lib")
endif(WIN32)

install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin"
                                 LIBRARY DESTINATION "bin"
                                 ARCHIVE DESTINATION "lib")
