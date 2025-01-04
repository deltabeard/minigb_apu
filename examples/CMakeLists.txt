cmake_minimum_required(VERSION 3.13...3.30)
project(minigb_apu_example)
add_executable(direct direct.c ../minigb_apu.c ../minigb_apu.h)
set(FREQ_INC_MULT 105 CACHE STRING "Fixed Point arithmetic compensation")
target_compile_definitions(direct PUBLIC
        AUDIO_SAMPLE_RATE=48000
        MINIGB_APU_AUDIO_FORMAT_S16SYS
        FREQ_INC_MULT=${FREQ_INC_MULT})
