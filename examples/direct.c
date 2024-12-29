#include "../minigb_apu.h"
#include "track.h"

#include <stdio.h>
#include <stdlib.h>

#define LOOP_MAX 5

int main(void)
{
	int ret = EXIT_FAILURE;
	int16_t samples[AUDIO_SAMPLES_TOTAL];
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

	audio_init(&apu_ctx);
	
	while(1)
	{
		switch(audio_frame_cmds[cmd].cmd)
		{
		case AUDIO_CMD_END_FRAME:
			cmd++;
			audio_callback(&apu_ctx, samples);
			fwrite(samples, sizeof(int16_t), AUDIO_SAMPLES_TOTAL, f);
			continue;

		case AUDIO_CMD_SET_REGISTER:
			audio_write(&apu_ctx, audio_frame_cmds[cmd].reg,
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
