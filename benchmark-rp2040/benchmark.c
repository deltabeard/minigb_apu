#include <pico/stdlib.h>
#include <hardware/sync.h>
#include <stdio.h>

#include "../minigb_apu.h"
#include "track.h"

#define LOOP_MAX 5

void process_apu(void)
{
	struct minigb_apu_ctx apu_ctx = {0};
	unsigned cmd = 0, loop = 0;
	int16_t samples[AUDIO_SAMPLES_TOTAL];
	audio_init(&apu_ctx);
	while(1)
	{
		switch(audio_frame_cmds[cmd].cmd)
		{
		case AUDIO_CMD_END_FRAME:
			cmd++;
			audio_callback(&apu_ctx, samples);
			continue;

		case AUDIO_CMD_SET_REGISTER:
			cmd++;
			continue;

		case AUDIO_CMD_LOOP:
			loop++;
			if(loop > LOOP_MAX)
				break;

			cmd = audio_frame_cmds[cmd].reg;
			continue;

		default:
			break;
		}

		break;
	}
}

void main(void)
{
	/* Use an accurate clock. */
	set_sys_clock_48mhz();

	stdio_init_all();

	while(1) {
		uint32_t bef, aft;
		uint32_t interrupts;
		while(getchar_timeout_us(1 * 1000 * 1000) == PICO_ERROR_TIMEOUT) {
			static unsigned wrap = 0;
			putchar_raw('.');
			wrap++;
			if(wrap >= 16) {
				putchar_raw('\n');
				wrap = 0;
			}
		}

		printf("Starting benchmark with sample rate %lu\n", AUDIO_SAMPLE_RATE);
		interrupts = save_and_disable_interrupts();
		bef = time_us_32();
		process_apu();
		aft = time_us_32();
		restore_interrupts(interrupts);
		printf("Duration: %lu us\n", aft - bef);
	}

	return;
}
