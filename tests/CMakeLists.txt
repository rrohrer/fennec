add_executable(dynamic_array_tests dynamic_array_tests.c)
target_link_libraries(dynamic_array_tests fennec)
add_test(dynamic_array dynamic_array_tests)

add_executable(hashtable_tests hashtable_tests.c)
target_link_libraries(hashtable_tests fennec)
add_test(hashtable hashtable_tests)

add_executable(string_tests string_tests.c)
target_link_libraries(string_tests fennec)
add_test(string string_tests)

add_executable(path_tests path_tests.c)
target_link_libraries(path_tests fennec)
add_test(path path_tests)