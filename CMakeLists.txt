cmake_minimum_required(VERSION 3.15)
project(QuickOK-IO 0.0.1-Alpha)

set(CMAKE_CXX_COMPILER OFF)
# For clangd
set(CMAKE_EXPORT_COMPILECOMMANDS ON)

# --------- QOZero ---------
if(NOT DEFINED QOZERO_SOURCE_ROOT)
    if(EXISTS ${CMAKE_SOURCE_DIR}/../QOZero)
        set(QOZERO_SOURCE_ROOT ${CMAKE_SOURCE_DIR}/../QOZero)
        message(STATUS "QOZERO_SOURCE_ROOT not set but QOZero found in the parent directory")
    else()
        message(FATAL_ERROR "QOZERO_SOURCE_ROOT not set and QOZero not found in the parent directory")
    endif()
endif()
target_include_directories(QORender PRIVATE ${QOZERO_SOURCE_ROOT}/include)
# --------------------------
