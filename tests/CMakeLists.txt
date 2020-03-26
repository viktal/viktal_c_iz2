add_executable(test_email test_email.cpp)
target_compile_options(test_email PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(test_email emailfilter_static gtest_main gcov)
add_test(NAME test_email COMMAND test_email)

add_executable(test_utils test_utils.cpp)
target_compile_options(test_utils PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(test_utils emailfilter_static gtest_main gcov)
add_test(NAME test_utils COMMAND test_utils)

add_executable(test_date test_date.cpp)
target_compile_options(test_date PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(test_date emailfilter_static gtest_main gcov)
add_test(NAME test_date COMMAND test_date)

add_executable(test_filter_static test_filter.cpp)
target_compile_options(test_filter_static PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(test_filter_static emailfilter_static gtest_main gcov)
add_test(NAME test_filter_static COMMAND test_filter_static)

add_executable(test_filter_shared test_filter.cpp)
target_compile_options(test_filter_shared PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(test_filter_shared emailfilter_shared gtest_main gcov)
add_test(NAME test_filter_shared COMMAND test_filter_shared)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/199911.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/201105.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stress-test.sh
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})