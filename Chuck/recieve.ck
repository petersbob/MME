// Make a receiver, set port#, set up to listen for event
OscIn oin;
7000 => oin.port;
OscMsg msg;
// create an address in the receiver, store in new variable
oin.addAddress( "/test1" );
// Our synthesizer to be controlled by sender process
Rhodey piano => dac;
SinOsc a => dac;
.5 => a.gain;
// Infinite loop to wait for messages and play notes
while (true)
{
 // OSC message is an event, chuck it to now
 oin => now;
 // when event(s) received, process them
 while (oin.recv(msg) != 0) {
   // peel off integer, float, string
   msg.getInt(0) => int note;
   msg.getFloat(1) => float velocity;
   msg.getString(2) => string howdy;
   velocity*3 + 400 => a.freq;
   // use them to make music
   //Std.mtof(note) => piano.freq;
   //velocity => piano.gain;
   //velocity => piano.noteOn;
   // print it all out
   <<< howdy, note, velocity >>>;
 }
}
