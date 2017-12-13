cmake_minimum_required(VERSION 3.5)

set( DB_SOURCES
    connection.cpp
    connection_impl.hpp
    ./include/hda/db/connection.hpp
    pool.cpp
    ./include/hda/db/pool.hpp
    settings.cpp
    ./include/hda/db/settings.hpp
    transaction.cpp
    transaction_impl.hpp
    ./include/hda/db/transaction.hpp
    ./include/hda/db/fwd.hpp
)

find_package(Threads REQUIRED)

find_path(PQXX_INCLUDE_DIR
    NAMES pqxx/pqxx
    DOC "The pqxx include directory"
)
find_library(PQXX_LIBRARY
    NAMES pqxx
    DOC "The pqxx library"
)
if (NOT PQXX_INCLUDE_DIR OR NOT PQXX_INCLUDE_DIR)
    message(ERROR "Cannot find pqxx")
endif()

include_directories("${PQXX_INCLUDE_DIR}")

add_library(dancebooks-db SHARED ${DB_SOURCES})
target_link_libraries(dancebooks-db ${PQXX_LIBRARY} Threads::Threads)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)


configure_file(tests/config.h.in tests/config.h @ONLY ESCAPE_QUOTES)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/tests)

file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "tests/*.cpp")
if (NOT HAVE_DB_SERVER)
    list(REMOVE_ITEM TEST_SOURCES "tests/test_connection_pool.cpp")
endif()

db_add_tests("${TEST_SOURCES}" dancebooks-db)
