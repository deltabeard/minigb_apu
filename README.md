# minigb_apu

Fast Game Boy audio emulation with 16‑bit sample output.
This library is designed for use on low power microcontrollers.
Hence, there are no floating point operations, and some variable are hard-coded at compile time.

## Examples
- [Peanut-SDL](https://github.com/deltabeard/Peanut-GB/tree/master/examples/sdl2).
- My fork of [MiniGBS](https://github.com/deltabeard/MiniGBS).

## Caveats

- A fixed number of audio samples (`AUDIO_SAMPLES`) must be read each frame.
- The sample rate must be set at compile time.

## Todo

- Perform benchmarks against other Game Boy APU emulators.
- Document inaccuracies in emulation.
