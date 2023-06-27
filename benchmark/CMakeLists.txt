# vim: ts=4:sw=4:expandtab
CMAKE_MINIMUM_REQUIRED(VERSION 3.14...3.26 FATAL_ERROR)

# Initialise project information.
PROJECT(apu-benchmark)
ADD_EXECUTABLE(apu-benchmark benchmark.cpp ../minigb_apu.c)

# Google Benchmark is required.
find_package(benchmark REQUIRED)
target_link_libraries(apu-benchmark benchmark::benchmark)

# To obtain track data.
target_include_directories(apu-benchmark PRIVATE ../examples)

