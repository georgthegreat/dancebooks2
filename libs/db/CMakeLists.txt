cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../tools/CMakeModules")
include(DanceBooksCommon)

find_package(Threads REQUIRED)
find_package(PQXX REQUIRED)

set(DB_SOURCES
    connection.cpp
    connection_impl.hpp
    pool.cpp
    settings.cpp
    transaction.cpp
    transaction_impl.hpp
)
set (DB_HEADERS
    ./include/hda/db/connection.hpp
    ./include/hda/db/pool.hpp
    ./include/hda/db/settings.hpp
    ./include/hda/db/transaction.hpp
    ./include/hda/db/fwd.hpp
)

add_library(dancebooks-db STATIC ${DB_SOURCES} ${DB_HEADERS})

include_directories("${PQXX_INCLUDE_DIR}")
target_link_libraries(dancebooks-db ${PQXX_LIBRARY} Threads::Threads)

db_add_test_dir(dancebooks-db "./tests" "config.h")