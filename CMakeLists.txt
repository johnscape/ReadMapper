cmake_minimum_required(VERSION 3.20)
project(genome)

set(CMAKE_CXX_STANDARD 14)

add_executable(genome main.cpp Cigar.cpp Cigar.h StringMatcher.cpp StringMatcher.h DataIO.cpp DataIO.h)
