cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools/CMakeModules")
include(DanceBooksCommon)

set(CPACK_PACKAGE_NAME "dancebooks2")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Bibliographic service for Historical Dance Association")
set(CPACK_PACKAGE_CONTACT "Yuriy Chernyshov <georgthegreat@gmail.com>")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(HAVE_DB_SERVER ON CACHE BOOL                        "If the PostgreSQL server is available to run tests on.")
set(DB_SERVER_HOST      "localhost" CACHE STRING        "PostgreSQL server hostname for tests")
set(DB_SERVER_PORT      5432 CACHE INT                  "PostgreSQL server port for tests")
set(DB_SERVER_USER      "hda_tester" CACHE STRING       "PostgreSQL server username for tests")
set(DB_SERVER_PASSWORD  "test_password" CACHE STRING    "PostgreSQL server password for tests")
set(DB_SERVER_DATABASE  "dancebooks_tests" CACHE STRING "PostgreSQL server DB name for tests")

#add_subdirectory(contrib/json)
include_directories("./libs/common/include")
add_subdirectory(libs/common)
include_directories("./libs/db/include")
add_subdirectory(libs/db)
