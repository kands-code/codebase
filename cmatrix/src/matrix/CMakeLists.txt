add_library(matrix STATIC
    "init_matrix.c"
)

target_link_libraries(matrix m)

target_include_directories(matrix PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")