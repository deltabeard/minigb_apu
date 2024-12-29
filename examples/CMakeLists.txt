cmake_minimum_required(VERSION 3.13...3.30)
project(minigb_apu_example)
add_executable(direct direct.c ../minigb_apu.c)
target_compile_definitions(direct PRIVATE AUDIO_SAMPLE_RATE=48000)
