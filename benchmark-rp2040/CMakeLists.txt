cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(benchmark-rp2040)

pico_sdk_init()

add_executable(benchmark-rp2040 benchmark.c ../minigb_apu.c)

# To obtain track data.
target_include_directories(benchmark-rp2040 PRIVATE ../examples)
target_compile_definitions(benchmark-rp2040 PRIVATE AUDIO_SAMPLE_RATE=48000 MINIGB_APU_AUDIO_FORMAT_S16SYS)
target_link_libraries(benchmark-rp2040 pico_stdlib hardware_sync hardware_clocks)

function(pico_add_verbose_dis_output TARGET)
	add_custom_command(TARGET ${TARGET} POST_BUILD
		COMMAND ${CMAKE_OBJDUMP} -h $<TARGET_FILE:${TARGET}> >$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.dis
		COMMAND ${CMAKE_OBJDUMP} -drwCSl $<TARGET_FILE:${TARGET}> >>$<IF:$<BOOL:$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>>,$<TARGET_PROPERTY:${TARGET},OUTPUT_NAME>,$<TARGET_PROPERTY:${TARGET},NAME>>.dis
		)
endfunction()

pico_set_binary_type(benchmark-rp2040 no_flash)
pico_enable_stdio_usb(benchmark-rp2040 1)
pico_add_bin_output(benchmark-rp2040)
pico_add_uf2_output(benchmark-rp2040)
pico_add_verbose_dis_output(benchmark-rp2040)
