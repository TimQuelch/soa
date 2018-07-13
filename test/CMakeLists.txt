find_package(Boost REQUIRED COMPONENTS unit_test_framework)

list(APPEND REQUIRED_FILES)
list(APPEND REQUIRED_INCLUDES)
list(APPEND REQUIRED_LIBS soa)
list(APPEND REQUIRED_FEATURES)
list(APPEND REQUIRED_OPTIONS -Wall -Wextra -Wpedantic -O2)

function(add_boost_test SOURCE_FILE_NAME)
    get_filename_component(TEST_EXECUTABLE_NAME ${SOURCE_FILE_NAME} NAME_WE)

    add_executable(${TEST_EXECUTABLE_NAME} ${SOURCE_FILE_NAME} ${REQUIRED_FILES})
    target_link_libraries(${TEST_EXECUTABLE_NAME} PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${REQUIRED_LIBS})
    target_compile_features(${TEST_EXECUTABLE_NAME} PRIVATE ${REQUIRED_FEATURES})
    target_compile_options(${TEST_EXECUTABLE_NAME} PRIVATE ${REQUIRED_OPTIONS})

    file(READ "${SOURCE_FILE_NAME}" SOURCE_FILE_CONTENTS)
    string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)"
        FOUND_TESTS ${SOURCE_FILE_CONTENTS})

    foreach(TEST ${FOUND_TESTS})
        string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${TEST})

        add_test(NAME "${TEST_EXECUTABLE_NAME}.${TEST_NAME}"
            COMMAND ${TEST_EXECUTABLE_NAME}
            --run_test=${TEST_NAME} --catch_system_error=yes)
    endforeach()
endfunction()

add_boost_test(soa-tests.cpp)
