CFLAGS := -O2 -g2 -Wall
SAMPLERATE := 48000
FREQ_INC_MULT := 105

override CFLAGS += -DAUDIO_SAMPLE_RATE=$(SAMPLERATE) \
	-DMINIGB_APU_AUDIO_FORMAT_S16SYS -DFREQ_INC_MULT=$(FREQ_INC_MULT)

all: direct
direct: direct.c ../minigb_apu.c
convert:
	ffmpeg -f s16le -ar 48000 -ac 2 -i audio.raw -filter:a "volume=0.5" -c:a libopus -b:a 128k audio.opus

clean:
	$(RM) direct.exe direct
