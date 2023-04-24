#include "../minigb_apu.h"
#include "track.h"
#include "mk_wav.h"

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
	struct mk_wav_ctx c;
	uint32_t bytes_written = 0;
	struct minigb_apu_ctx apu_ctx = {0};

	f = fopen("audio.wav", "wb");
	if(f == NULL)
	{
		perror("Unable to open output file");
		goto out;
	}

	ret = mk_wav_init(&c, f, 16, 2, AUDIO_SAMPLE_RATE);
	assert(ret == 0);

	audio_init(&apu_ctx);
	
	while(1)
	{
		switch(audio_frame_cmds[cmd].cmd)
		{
		case AUDIO_CMD_END_FRAME:
			cmd++;
			audio_callback(&apu_ctx, samples);
			//drwav_write_raw(&pWav, sizeof(samples), samples);
			mk_wav_write(&c, samples, sizeof(samples));
			bytes_written += sizeof(samples);
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

	//drwav_uninit(&pWav);
	mk_wav_exit(&c);
	fclose(f);

	ret = EXIT_SUCCESS;
out:
	return ret;
}	
