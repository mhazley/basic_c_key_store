cmake_minimum_required(VERSION 3.14)
project(key_store)

set(CMAKE_C_STANDARD 11)

add_executable(key_store_write main.c key_store.c key_store.h)
add_executable(key_store_read main2.c key_store.c key_store.h)