cmake_minimum_required(VERSION 3.5)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

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

macro(db_add_test_dir LIB_NAME DIR CONFIG_FILES)
    foreach(_cf ${CONFIG_FILES})
        configure_file(${DIR}/${_cf}.in ${DIR}/${_cf} @ONLY ESCAPE_QUOTES)
    endforeach()
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/${DIR})

    file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${DIR}/*.cpp")
    db_add_tests("${TEST_SOURCES}" ${LIB_NAME})
endmacro()

