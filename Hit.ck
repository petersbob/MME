SndBuf AudioFile1 => dac;
int i;

["Audio/cymbal1.wav", "Audio/cymbal2.wav", "Audio/cymbal3.wav"] @=> string hit[];
Math.random2(0, 2) => i;

me.dir()+hit[i] => AudioFile1.read;
0.5 => AudioFile1.gain;

0 => AudioFile1.pos; 
1 => AudioFile1.rate;

AudioFile1.samples() :: samp => now; 