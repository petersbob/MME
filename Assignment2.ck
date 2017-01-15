//Caleb Goss
//Assignment 2

//This project is a random harmony generator. What I find particularly
//interesting is that the harmonic motion (pretty much) always sounds good,
//even weird changes that wouldn't make sense in acoustic music. The 
//reason, as I see it, is because acoustic harmony relies on good voice
//leading. However, in this arrangment, we are hearing a scattered cloud 
//of notes over a broad range that completely eliminates the concept of voice 
//leading in a single line. Instead, we hear all sections of the cloud shifting 
//together. This effect is amplified by shortening the duration of each note.

TriOsc t => dac;  
TriOsc v => dac;//chuck triangle oscillator to DAC
SinOsc u =>  dac; //chuck sin oscillator to DAC
0.35 => t.gain;   //Set gain of triangle oscillator

int chordType;    //refers to x index of the two-d array below
int chordMember1;
int chordMember2;  //refers to y index of the two-d array below
int chordRoot;    //used to determine the root of the chord
int offset;       //used to keep the range centered
float HzOne;      //updates frequency of triangle oscillator
float HzTwo;      //updates frequency of sin oscillator
float HzThree;
float gainInc;    //increments/decrements sin oscillator gain

//This array of arrays contains the midi note values corresponding to the
//chord members for the following chord types (all in C): major, minor, 
//major7, minor7, dominant, diminished, half-diminished

[[48, 48, 52, 55, 60, 64, 67, 72, 72], [48, 48, 51, 55, 60, 63, 67, 72, 72],
[48, 52, 55, 59, 60, 64, 67, 71, 72], [48, 51, 55, 58, 60, 63, 67, 70, 72],
[48, 52, 55, 58, 60, 64, 67, 70, 72], [48, 51, 54, 57, 60, 63, 66, 69, 72],
[48, 51, 54, 58, 60, 63, 66, 70, 72]] @=> int notes[][];

//The main loop in our code runs 16 times, so we will hear as many chord 
//changes as the program runs
for (0 => int i; i < 12; i++)
{
    //set the gain at 0 at the beginning of each chord change
    0 => gainInc;                    
    
    Math.random2(0, 6) => chordType;  //randomly generate chord quality
    Math.random2(0, 10) => chordRoot; //randomly generate chord root
    
    //This for loop plays individual notes, and increments/decrements the base gain
    for (0 => int j; j < 30; j++)
    {
        
        //this if/else is what causes the base to fade in/out
        if(j < 20)
        {
            gainInc + .02 => gainInc;      // gain goes up in the bass for 2/3 of the time the chord is played
        }
        else
        {
            gainInc - .03 => gainInc;      // and down more quickly in the last third
        }
        
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
        gainInc => u.gain;  //chuck incremented gain value to sin oscillator
        100::ms => now;     //play notes for 125 ms
    }
}