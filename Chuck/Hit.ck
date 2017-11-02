fun void harmony(int bass)
{
    TriOsc t => JCRev r => dac;  
    TriOsc v => r => dac;//chuck triangle oscillator to DAC
    SinOsc u => ADSR e => dac; //chuck sin oscillator to DAC
    //Set gain of triangle oscillator
    
    e.set(50::ms, 500::ms, .1, 500::ms);
    .1 => r.mix;
    
    int chordType;    //refers to x index of the two-d array below
    int chordMember1;
    int chordMember2;  //refers to y index of the two-d array below
    int chordRoot;    //used to determine the root of the chord
    int offset;       //used to keep the range centered
    float HzOne;      //updates frequency of triangle oscillator
    float HzTwo;      //updates frequency of sin oscillator
    float HzThree;
    .125 => float gain;
    gain => t.gain => v.gain; 
    //increments/decrements sin oscillator gain
    .5 => u.gain;
    //This array of arrays contains the midi note values corresponding to the
    //chord members for the following chord types (all in C): major, minor, 
    //major7, minor7, dominant, diminished, half-diminished
    
    [[48, 48, 52, 55, 60, 64, 67, 72, 72], [48, 48, 51, 55, 60, 63, 67, 72, 72],
    [48, 52, 55, 59, 60, 64, 67, 71, 72], [48, 51, 55, 58, 60, 63, 67, 70, 72],
    [48, 52, 55, 58, 60, 64, 67, 70, 72], [48, 51, 54, 57, 60, 63, 66, 69, 72],
    [48, 51, 54, 58, 60, 63, 66, 70, 72]] @=> int notes[][];
    
    
    Math.random2(0, 6) => chordType;  //randomly generate chord quality
    Math.random2(0, 10) => chordRoot; //randomly generate chord root
    
    //This for loop plays individual notes, and increments/decrements the base gain
    for (0 => int j; j < 30; j++)
    {
        
        //this if/else is what causes the base to fade in/out
        
        
        Math.random2(0, 8) => chordMember1;
        Math.random2(0, 8) => chordMember2; //randomly generate which member of the chord to play
        
        
        //this if/else keeps us in the center of the keyboard and converts
        //our midi note number to its corresponding frequency
        if (chordRoot < 6)
        {
            Std.mtof(notes[chordType][chordMember1] + chordRoot) => HzOne; //MIDI -> frequency for the bouncy voice
            Std.mtof(notes[chordType][chordMember2] + chordRoot) => HzThree;
            Std.mtof(notes[chordType][0] + chordRoot - 12) => HzTwo;      //MIDI -> frequency for the bass voice
        }
        else
        {
            12 - chordRoot => offset;                                     //centers the chord
            Std.mtof(notes[chordType][chordMember1] - offset) => HzOne;  
            Std.mtof(notes[chordType][chordMember2] - offset) => HzThree;  //MIDI -> frequency for the bouncy voice
            Std.mtof(notes[chordType][0] + chordRoot - 12) => HzTwo;      //MIDI -> frequency for the bass voice
        }
        HzOne => t.freq; 
        HzTwo => v.freq;   //update frequency of triangle oscillator
        HzThree => u.freq;    //update frequency of sin oscillator
        //chuck incremented gain value to sin oscillator
        150::ms => now;     //play notes for 125 ms
        if (gain > .005)
        {
            gain - .01 => gain;
            gain => t.gain => v.gain;
        }
    }
}
fun void cymbal()
{
    SndBuf AudioFile1 => JCRev r => dac;
    Std.rand2f(.2, .4) => r.mix;
    
    ["Audio/cymbal1.wav", "Audio/cymbal2.wav", "Audio/cymbal3.wav"] @=> string hit[];
    
    me.dir()+hit[Math.random2(0, 2)] => AudioFile1.read;
    .25 => AudioFile1.gain;
    0 => AudioFile1.pos; 
    Std.rand2f(.75, 1.25) => AudioFile1.rate;
    
    AudioFile1.samples()::samp => now;
}

int root;

Math.random2(0, 10)=> root;

spork ~ cymbal();
spork ~ harmony(root);

7::second => now;