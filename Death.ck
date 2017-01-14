SndBuf AudioFile1 => JCRev r => dac;
.5 => r.mix;

me.dir()+"/Audio/bass.wav" => AudioFile1.read;
5.0 => AudioFile1.gain;

0 => AudioFile1.pos; 
1 => AudioFile1.rate;

5 :: second => now; 