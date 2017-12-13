cmake_minimum_required(VERSION 3.5)

set( COMMON_SOURCES
    exception.cpp
    ./include/hda/common/exception.hpp
    ./include/hda/common/pimpl_utils.hpp
)

add_library(dancebooks-common SHARED ${COMMON_SOURCES})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "tests/*.cpp")
db_add_tests("${TEST_SOURCES}" dancebooks-common)
