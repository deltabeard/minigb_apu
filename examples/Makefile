CFLAGS := -O2 -g2 -Wall
SAMPLERATE := 48000
FREQ_INC_MULT := 105

override CFLAGS += -DAUDIO_SAMPLE_RATE=$(SAMPLERATE) \
	-DMINIGB_APU_AUDIO_FORMAT_S16SYS -DFREQ_INC_MULT=$(FREQ_INC_MULT)

all: direct
direct: direct.c ../minigb_apu.c

clean:
	$(RM) direct.exe direct
