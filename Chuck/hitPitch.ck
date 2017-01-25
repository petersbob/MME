// ADSR envelope
// by Ge Wang et al

SinOsc s => LPF lp => ADSR e => JCRev r =>dac;
SinOsc t => lp => e => dac;
SinOsc lfo => dac;

.3 => r.mix;

float theValue;

8000 => lp.freq;
5.0 => lp.Q;
3.0 => lfo.gain;
4.0 => lfo.freq; 
lfo => blackhole;

// set a, d, s, and r
e.set( 10::ms, 8::ms, .5, 500::ms );
// set gain
.5 => s.gain;

for (0 => int i; i < 10; i++)
{
    // choose freq
    Math.random2(40, 60) => Std.mtof => s.freq => t.freq;
    
    // key on - start attack
    e.keyOn();
    
    for (0 => int i; i < 200; i++)
    {
        //LFO update
        lfo.last() => theValue;   
        (theValue + 1) / 2 => theValue;   
        theValue => s.gain => t.gain;
        
      
        ms => now;
       
    }
    // key off - start release
    e.keyOff();
    
    // advance time by 800 ms
    800::ms => now;
}