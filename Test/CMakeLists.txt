add_executable (Test src/test_unbounded_int.c)
target_link_libraries (Test PRIVATE Objects Library)

add_test(NAME src/test_unbounded_int.c COMMAND test_unbounded_int)