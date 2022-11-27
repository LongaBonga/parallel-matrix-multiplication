project(parallel-matrix-multiplication)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MatMulPar main.cpp utils.cpp utils.h)

