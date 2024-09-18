cmake_minimum_required(VERSION 3.26)
project(TrueSecretKey)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS -static)

add_executable(TrueSecretKey main.cpp res.rc)
