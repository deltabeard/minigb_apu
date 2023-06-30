# minigb_apu

Fast Game Boy audio emulation with 16‑bit signed sample output.
This library is designed for use on low power microcontrollers.
Hence, there are no floating point operations, and some variables are hard-coded at compile time.

This project uses code from [baines/MiniGBS](https://github.com/baines/MiniGBS).

## Examples

- [./examples/](./examples/): Processes track data and outputs audio samples to a WAV file.
- [./benchmark/](./benchmark/): Uses [Google Benchmark](https://github.com/google/benchmark) to benchmark the APU.
- [./benchmark-rp2040/](./benchmark-rp2040/): Measures the duration of time required to play the example track on a Raspberry Pi Pico (RP2040).
- [Peanut-SDL](https://github.com/deltabeard/Peanut-GB/tree/master/examples/sdl2): Used as the APU for a Game Boy emulator.
- [deltabeard/MiniGBS](https://github.com/deltabeard/MiniGBS): Use as the APU for a GBS player.

## Caveats

- A fixed number of audio samples (`AUDIO_SAMPLES`) must be read each frame.
- The sample rate must be set at compile time. This is set to 32768 Hz by default and can be changed by defining `AUDIO_SAMPLE_RATE`.

## Todo

- Perform benchmarks against other Game Boy APU emulators.
- Document inaccuracies in emulation.

# License

MIT. See the LICENSE file.
