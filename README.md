MIDI code I am working on. MIDI.cxx is a program which plays sounds using MIDI input (requiring a MIDI keyboard)
by passing the MIDI messages through Microsofts GS Wavetable Synth.

There is a noticeable delay between key presses and the audio being played. The reason for this delay is explained well in this link:
https://learn.microsoft.com/en-us/windows-hardware/drivers/audio/midi-and-directmusic-components

"However, by emulating the behavior of older software and hardware, the midiOutXxx and midiInXxx functions sacrifice the precision timing and enhanced functionality that are now available through the DirectMusic API"

So it's better to explore the DirectMusic API.
