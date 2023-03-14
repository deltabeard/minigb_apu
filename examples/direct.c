#include "../minigb_apu.h"
#include "track.h"

#define DR_WAV_IMPLEMENTATION
#define DR_WAV_NO_STDIO
#define DR_WAV_NO_CONVERSION_API
#include "dr_wav.h"

#include <stdio.h>
#include <stdlib.h>

#define LOOP_MAX 5
#define AUDIO_LENGTH_SAMPLES	(AUDIO_SAMPLE_RATE * AUDIO_LENGTH_SECONDS)

struct userdata {
	FILE *out;
};

size_t wav_write(void* pUserData, const void* pData, size_t bytesToWrite)
{
	struct userdata *u = pUserData;
	return fwrite(pData, 1, bytesToWrite, u->out);
}

drwav_bool32 wav_seek(void* pUserData, int offset, drwav_seek_origin origin)
{
	struct userdata *u = pUserData;
	return fseek(u->out, offset, origin);
}

int main(void)
{
	int ret = EXIT_FAILURE;
	int16_t samples[AUDIO_SAMPLES_TOTAL];
	unsigned cmd = 0;
	int loop = 0;
	drwav pWav;
	drwav_bool32 dret;
	struct userdata u;
	uint32_t bytes_written = 0;

	const drwav_data_format pFormat = {
		drwav_container_riff,
		DR_WAVE_FORMAT_PCM,
		2,
		AUDIO_SAMPLE_RATE,
		16
	};

	u.out = fopen("audio.wav", "wb");
	if(u.out == NULL)
	{
		perror("Unable to open output file");
		goto out;
	}

	dret = drwav_init_write(&pWav, &pFormat, wav_write, wav_seek, &u, NULL);
	assert(dret == DRWAV_TRUE);

	audio_init();
	
	while(1)
	{
		switch(audio_frame_cmds[cmd].cmd)
		{
		case AUDIO_CMD_END_FRAME:
			cmd++;
			audio_callback(NULL, samples, sizeof(samples));
			drwav_write_raw(&pWav, sizeof(samples), samples);
			bytes_written += sizeof(samples);
			continue;

		case AUDIO_CMD_SET_REGISTER:
			audio_write(audio_frame_cmds[cmd].reg,
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

	drwav_uninit(&pWav);

	fseek(u.out, 40, SEEK_SET);
	fwrite(&bytes_written, sizeof(uint32_t), 1, u.out);
	fclose(u.out);

	ret = EXIT_SUCCESS;
out:
	return ret;
}	
