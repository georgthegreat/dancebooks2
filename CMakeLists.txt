cmake_minimum_required(VERSION 3.5)

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

macro(db_add_tests TEST_SOURCES LIB_NAME)
    foreach(_f ${TEST_SOURCES})
        get_filename_component(_t ${_f} NAME_WE)
        set(TEST_NAME test-${LIB_NAME}-${_t})
        add_executable(${TEST_NAME} ${_f})
        target_link_libraries(${TEST_NAME} ${LIB_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        add_custom_command(
            TARGET ${TEST_NAME}
            COMMENT "Run ${TEST_NAME}"
            POST_BUILD
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failures -R "^${TEST_NAME}$"
        )
    endforeach()
endmacro()

include_directories("./libs/common/include")
add_subdirectory(libs/common)
include_directories("./libs/db/include")
add_subdirectory(libs/db)
