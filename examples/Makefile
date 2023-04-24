CFLAGS := -O2 -g2 -Wall
SAMPLERATE := 48000

override CFLAGS += -DAUDIO_SAMPLE_RATE=$(SAMPLERATE)

all: direct
direct: direct.c ../minigb_apu.c

clean:
	$(RM) direct.exe direct
