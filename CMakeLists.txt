cmake_minimum_required(VERSION 3.12)
project(SimpleLibrarySystem C)

# 强制使用 C23 标准
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 开启调试信息和警告
add_compile_options(-Wall -Wextra -g -pedantic)

# 包含头文件路径
include_directories(src)

# 源文件定义
set(SOURCES
    src/main.c
    src/data.c
    src/store.c
    src/logic.c
)

add_executable(library_sys ${SOURCES})