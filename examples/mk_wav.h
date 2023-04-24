/**
 * Simple WAV writer for little endian computers.
 * Copyright (c) 2023 Mahyar Koshkouei
 *
 * Licence: Public Domain
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 * THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED WARRANTY.
 * IN NO EVENT WILL THE AUTHORS BE HELD LIABLE FOR ANY DAMAGES ARISING FROM THE
 * USE OF THIS SOFTWARE.
 */

#pragma once

#include <assert.h>
#include <stdio.h>

struct mk_wav_ctx {
	FILE *out;
	size_t bytes;
};

/**
 * Initialise WAV writer library. FILE must already be opened and seekable.
 *
 * \param c Pointer to a mk_wav context.
 * \param out Pointer to opened and writable file pointer.
 * \param bits_per_sample
 * \param channels
 * \param sample_rate
 * \returns 0 on success, negative on error.
 */
int mk_wav_init(struct mk_wav_ctx *c, FILE *out, unsigned short bits_per_sample,
		unsigned short channels, unsigned long sample_rate);

/**
 * Write audio samples to output file.
 * \param c An initialised mk_wav context.
 * \param samples Data samples to write to output WAV file.
 * \param len Number of bytes in samples.
 * \returns bytes written; equal to len on success.
 */
size_t mk_wav_write(struct mk_wav_ctx *c, void *samples, size_t len);

/**
 * Updates the file size in the WAV header. FILE is not closed.
 * \param c An initialised mk_wav context.
 * \returns 0 on success, negative on error.
 */
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

	/* Write file headers. */
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

	/* Write samples directly to WAV file. */
	return fwrite(samples, 1, len, c->out);
}

long mk_wav_exit(struct mk_wav_ctx *c)
{
#define WAV_RIFF_SIZE_LOC 0x04
#define WAV_DATA_SIZE_LOC 0x28
#define WAV_RIFF_SIZE_COMPENSATION 0x08
#define WAV_DATA_SIZE_COMPENSATION 0x2C
	long sz;
	size_t riff_sz, data_sz;

	assert(c != NULL);
	assert(c->out != NULL);

	/* Obtain file size. */
	sz = ftell(c->out);
	if(sz < 0)
		return sz;

	/* Calculate size of RIFF and DATA chunks. */
	riff_sz = sz - WAV_RIFF_SIZE_COMPENSATION;
	data_sz = sz - WAV_DATA_SIZE_COMPENSATION;

	/* Update RIFF and DATA chunk sizes in WAV header. */
	fseek(c->out, WAV_RIFF_SIZE_LOC, SEEK_SET);
	fwrite(&riff_sz, 4, 1, c->out);
	fseek(c->out, WAV_DATA_SIZE_LOC, SEEK_SET);
	fwrite(&data_sz, 4, 1, c->out);

	/* Seek to end of file and return. */
	return fseek(c->out, 0, SEEK_END);
#undef WAV_RIFF_SIZE_LOC
#undef WAV_DATA_SIZE_LOC
#undef WAV_RIFF_SIZE_COMPENSATION
#undef WAV_DATA_SIZE_COMPENSATION
}
