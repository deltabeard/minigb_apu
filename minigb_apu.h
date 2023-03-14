/**
 * minigb_apu is released under the terms listed within the LICENSE file.
 *
 * minigb_apu emulates the audio processing unit (APU) of the Game Boy. This
 * project is based on MiniGBS by Alex Baines: https://github.com/baines/MiniGBS
 */

#pragma once

#include <stdint.h>

#ifndef AUDIO_SAMPLE_RATE
# define AUDIO_SAMPLE_RATE	32768
#endif

#define DMG_CLOCK_FREQ		4194304.0
#define SCREEN_REFRESH_CYCLES	70224.0
#define VERTICAL_SYNC		(DMG_CLOCK_FREQ/SCREEN_REFRESH_CYCLES)

/* Number of audio samples in each channel. */
#define AUDIO_SAMPLES		((unsigned)(AUDIO_SAMPLE_RATE / VERTICAL_SYNC))
/* Number of audio channels. The audio output is in interleaved stereo format.*/
#define AUDIO_CHANNELS		2
/* Number of audio samples output in each audio_callback call. */
#define AUDIO_SAMPLES_TOTAL	(AUDIO_SAMPLES * 2)

#define AUDIO_MEM_SIZE		(0xFF3F - 0xFF10 + 1)
#define AUDIO_ADDR_COMPENSATION	0xFF10

struct chan_len_ctr {
	uint8_t load;
	unsigned enabled : 1;
	uint32_t counter;
	uint32_t inc;
};

struct chan_vol_env {
	uint8_t step;
	unsigned up : 1;
	uint32_t counter;
	uint32_t inc;
};

struct chan_freq_sweep {
	uint16_t freq;
	uint8_t rate;
	uint8_t shift;
	unsigned up : 1;
	uint32_t counter;
	uint32_t inc;
};

struct chan {
	unsigned enabled : 1;
	unsigned powered : 1;
	unsigned on_left : 1;
	unsigned on_right : 1;
	unsigned muted : 1;

	uint8_t volume;
	uint8_t volume_init;

	uint16_t freq;
	uint32_t freq_counter;
	uint32_t freq_inc;

	int_fast16_t val;

	struct chan_len_ctr    len;
	struct chan_vol_env    env;
	struct chan_freq_sweep sweep;

	union {
		struct {
			uint8_t duty;
			uint8_t duty_counter;
		} square;
		struct {
			uint16_t lfsr_reg;
			uint8_t  lfsr_wide;
			uint8_t  lfsr_div;
		} noise;
		struct {
			uint8_t sample;
		} wave;
	};
};

struct minigb_apu_ctx {
	/**
	 * Memory holding audio registers between 0xFF10 and 0xFF3F inclusive.
	 */
	uint8_t audio_mem[AUDIO_MEM_SIZE];
	struct chan chans[4];
	int32_t vol_l, vol_r;
};

/**
 * Fill allocated buffer "stream" with AUDIO_SAMPLES_TOTAL number of 16-bit
 * signed samples (native endian order) in stereo interleaved format.
 * "sz" must be equal to AUDIO_SAMPLES_TOTAL.
 * Each call corresponds to the the time taken for each VSYNC in the Game Boy.
 *
 * \param userdata Unused. Only kept for ease of use with SDL2.
 * \param stream Allocated pointer to store audio samples. Must be at least
 *		AUDIO_SAMPLES_TOTAL in size.
 * \param sz Size of stream. Must be AUDIO_SAMPLES_TOTAL.
 */
void audio_callback(struct minigb_apu_ctx *ctx,
		int16_t stream[static AUDIO_SAMPLES_TOTAL]);

/**
 * Read audio register at given address "addr".
 */
uint8_t audio_read(struct minigb_apu_ctx *ctx, const uint16_t addr);

/**
 * Write "val" to audio register at given address "addr".
 */
void audio_write(struct minigb_apu_ctx *ctx,
		const uint16_t addr, const uint8_t val);

/**
 * Initialise audio driver.
 */
void audio_init(struct minigb_apu_ctx *ctx);
