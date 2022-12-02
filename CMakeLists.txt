project(parallel-matrix-multiplication)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pthreads-w32)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sys_time)

add_executable(MatMulPar main.cpp utils.cpp utils.h)

