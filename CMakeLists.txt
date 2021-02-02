cmake_minimum_required(VERSION 3.10)

set(PROJ_NAME "dggt")
set(SRC_DIR "src")
set(DEBUG_OPTS "-ggdb;-DDEBUG;-Wall;-Werror;-Wno-unused-variable")
set(RELEASE_OPTS "-DRELEASE;-Wall;-Werror")
set(SRC_FILES
	${SRC_DIR}/dggt_allocator.cxx
	${SRC_DIR}/dggt_math.cxx
	${SRC_DIR}/dggt_mem_utils.cxx
	${SRC_DIR}/dggt_test.cxx)

project(${PROJ_NAME})

add_compile_options(-std=c++17)
add_compile_options("$<$<CONFIG:Debug>:>${DEBUG_OPTS}")
add_compile_options("$<$<CONFIG:Release>:${RELEASE_OPTS}>")

add_executable(dggt ${SRC_FILES})
