#include <benchmark/benchmark.h>

extern "C" {
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
}

static void BM_MiniGB_APU(benchmark::State& state)
{
	for(auto _ : state)
	{
		process_apu();
	}
}

BENCHMARK(BM_MiniGB_APU);
BENCHMARK_MAIN();
