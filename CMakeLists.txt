cmake_minimum_required(VERSION 3.27)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "Generate compile_commands")

##Change projectName
project(HTTPServer)

set(CMAKE_C_STANDARD 11)
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
add_compile_options(-Wall -Wextra -pedantic -Werror -Wunused)

add_executable(${PROJECT_NAME}
   src/main.c
   src/server.c)
