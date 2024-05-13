cmake_minimum_required(VERSION 3.12)
project(se_comp_arch24_cache_isa_bialger)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        argparser
        riscv_asm
        cache
)

add_subdirectory(libraries)
