cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/CMakeModules")
include(DanceBooksCommon)

set(COMMON_SOURCES
    exception.cpp
)
set(COMMON_HEADERS
    ./include/hda/common/exception.hpp
    ./include/hda/common/pimpl_utils.hpp
)

add_library(dancebooks-common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})

db_add_test_dir( dancebooks-common "./tests" "")
