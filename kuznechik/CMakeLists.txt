cmake_minimum_required(VERSION 3.7)
project(kuznechik)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c kuznechik.c sbox.h galios_arithmetics.c galois_arithmetics.h integralmethods.c integral_funcs.h)
add_executable(kuznechik ${SOURCE_FILES})