#include "../minigb_apu.h"
#include "track.h"

#include <stdio.h>
#include <stdlib.h>

#define LOOP_MAX 5

int main(void)
{
	int ret = EXIT_FAILURE;
	unsigned cmd = 0;
	int loop = 0;
	FILE *f;
	struct minigb_apu_ctx apu_ctx;

	f = fopen("audio.raw", "wb");
	if(f == NULL)
	{
		perror("Unable to open output file");
		goto out;
	}

	minigb_apu_audio_init(&apu_ctx);
	
	while(1)
	{
#define SAMPLES_N 32
		audio_sample_t samples[SAMPLES_N];

		switch(audio_frame_cmds[cmd].cmd)
		{
		case AUDIO_CMD_END_FRAME:
			cmd++;
			for (unsigned i = 0; i < AUDIO_SAMPLES_TOTAL; i += SAMPLES_N)
			{
				minigb_apu_audio_callback(&apu_ctx, samples, SAMPLES_N);
				fwrite(samples, sizeof(audio_sample_t), SAMPLES_N, f);
			}
			continue;

		case AUDIO_CMD_SET_REGISTER:
			minigb_apu_audio_write(&apu_ctx, audio_frame_cmds[cmd].reg,
					audio_frame_cmds[cmd].val);
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

	fclose(f);

	ret = EXIT_SUCCESS;
out:
	return ret;
}	
