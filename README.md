MIDI code I am working on. MIDI.cxx is a program which plays sounds using MIDI input (requiring a MIDI keyboard)
by passing the MIDI messages through Microsofts GS Wavetable Synth.

There is a noticeable delay between key presses and the audio being played. I believe this is due to the underlying
API which the Synthesizer uses. I intend on testing this by making a similar program which instead uses WASAPI
together with a custom synth to see if improves the delay.

WASAPI is the underlying API used by DirectSound, DirectMusic, waveXxx functions etc (ref https://learn.microsoft.com/en-us/windows/win32/coreaudio/about-the-windows-core-audio-apis)

First I will see if using the MultiMedia waveXxx functions provide any improvements in performance.
