#pragma once

#include <assert.h>
#include <stdio.h>

struct mk_wav_ctx {
	FILE *out;
	size_t bytes;
};

/* Initialise WAV writer library. */
int mk_wav_init(struct mk_wav_ctx *c, FILE *out, unsigned short bits_per_sample,
		unsigned short channels, unsigned long sample_rate);

/* Write audio samples to output file. */
size_t mk_wav_write(struct mk_wav_ctx *c, void *samples, size_t len);

/* Exit WAV writer library. FILE is not closed. */
long mk_wav_exit(struct mk_wav_ctx *c);


int mk_wav_init(struct mk_wav_ctx *c, FILE *out, unsigned short bits_per_sample,
		unsigned short channels, unsigned long sample_rate)
{
	int ret = -1;
	const char hdr[] = "RIFF\0\0\0\0WAVEfmt"
		"\x20\x10\0\0\0"
		"\1"; /* C adds \0 to end of string. */
	const char data[] = "data\0\0\0";

	/* Initialise context. */
	c->out = out;
	c->bytes = 0;

	if(fwrite(hdr, 1, sizeof(hdr), out) < sizeof(hdr))
		goto out;

	if(fwrite(&channels, 2, 1, out) < 1)
		goto out;

	if(fwrite(&sample_rate, 4, 1, out) < 1)
		goto out;

	{
		unsigned long byte_rate;
		byte_rate = (sample_rate * channels * bits_per_sample) / 8ul;
		if(fwrite(&byte_rate, 4, 1, out) < 1)
			goto out;
	}

	{
		unsigned short block_align;
		block_align = (channels * bits_per_sample) / 8ul;
		if(fwrite(&block_align, 2, 1, out) < 1)
			goto out;
	}

	if(fwrite(&bits_per_sample, 2, 1, out) < 1)
		goto out;

	if(fwrite(data, 1, sizeof(data), out) < sizeof(data))
		goto out;

	if(ftell(out) != 44)
		goto out;

	ret = 0;
out:
	return ret;
}

size_t mk_wav_write(struct mk_wav_ctx *c, void *samples, size_t len)
{
	assert(c != NULL);
	assert(c->out != NULL);
	assert(samples != NULL);
	return fwrite(samples, 1, len, c->out);
}

long mk_wav_exit(struct mk_wav_ctx *c)
{
#define WAV_RIFF_SIZE_LOC 0x04
#define WAV_DATA_SIZE_LOC 0x28
	long sz;
	size_t riff_sz, data_sz;

	assert(c != NULL);
	assert(c->out != NULL);
	sz = ftell(c->out);
	if(sz < 0)
		return sz;

	riff_sz = sz - 8ul;
	data_sz = sz - 78ul;
	fseek(c->out, WAV_RIFF_SIZE_LOC, SEEK_SET);
	fwrite(&riff_sz, 4, 1, c->out);
	fseek(c->out, WAV_DATA_SIZE_LOC, SEEK_SET);
	fwrite(&data_sz, 4, 1, c->out);

	return fseek(c->out, 0, SEEK_END);
}
