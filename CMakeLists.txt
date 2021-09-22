cmake_minimum_required(VERSION 3.20)
project(ReadMapper)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

add_executable(readmapper main.cpp Cigar.cpp Cigar.h StringMatcher.cpp StringMatcher.h DataIO.cpp DataIO.h)
