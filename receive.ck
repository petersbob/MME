int oldStock;
int newStock;
float height;

SinOsc a => dac;
.6 => a.gain;

int hitLagFrames;
// Make a receiver, set port#, set up to listen for event
OscIn oin;
7000 => oin.port;
OscMsg msg;
// create an address in the receiver, store in new variable
oin.addAddress( "/playerInfo" );

// Infinite loop to wait for messages and play notes
while (true)
{
    // OSC message is an event, chuck it to now
    oin => now;
    // when event(s) received, process them
    while (oin.recv(msg) != 0)
    {
      // peel off integer, float, string
        msg.getString(0) => string dataTitle;
        if (dataTitle == "player_one_y")
        {
            msg.getFloat(1) => height;
            height + 400 => a.freq;
        }
        if (dataTitle == "player_one_hitlag_frames_left")
        {
            msg.getInt(1) => hitLagFrames;
            if (hitLagFrames > 0 && hitLagFrames < 2)
            {
                Machine.add( "Hit.ck" );
            }
        }
        if (dataTitle == "player_one_stock")
        {
            msg.getInt(1) => newStock;
            <<< "player_one_stock: ", newStock >>>;
            if (oldStock - 1 == newStock) 
            {
                Machine.add( "Death.ck" );
            }
            newStock => oldStock;
        }
        
        msg.getFloat(1) => float velocity;
        msg.getString(2) => string howdy;
        //<<< howdy, note, velocity >>>;
    }
}
