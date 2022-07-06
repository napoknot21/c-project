add_executable(Calc ${CMAKE_CURRENT_SOURCE_DIR}/src/calc_unbounded_int.c)

target_link_libraries(Calc PRIVATE Library)
target_link_libraries(Calc PRIVATE Objects)