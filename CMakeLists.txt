cmake_minimum_required(VERSION 3.10)

set(PROJ_NAME,"dggt")
set(SRC_DIR,"src")

project(${PROJ_NAME})

add_compile_options($<$<CONFIG:DEBUG>:-ggdb:-DDEBUG>)

add_executable(dggt ${SRC_DIR}/dggt_allocator.cxx ${SRC_DIR}/dggt_math.cxx ${SRC_DIR}/dggt_mem_utils.cxx ${SRC_DIR}/dggt_test.cxx)
