int player_two_old_stock;
int player_two_old_percent;
float player_two_height;

TriOsc a => JCRev r => dac;
.5 => r.mix;

OscMsg player_one_msg;
OscMsg player_two_msg;

int hitLagFrames;
// Make a receiver, set port#, set up to listen for event

OscIn player_two_oin;
7000 => player_two_oin.port;

// create an address in the receiver, store in new variable
player_two_oin.addAddress( "/player_two_info" );

// Infinite loop to wait for messages and play notes
while (true)
{
    player_two_oin => now;
    // when event(s) received, process them
    while (player_two_oin.recv(player_two_msg) != 0)
    {
        // peel off integer, float, string
        player_two_msg.getString(0) => string dataTitle;
        if (dataTitle == "player_y")
        {
            player_two_msg.getFloat(1) => player_two_height;
            if (player_two_height > 5)
            {
              .1 => a.gain;
              player_two_height + 400 => a.freq;
            }
            else
            {
              0 => a.gain;
            }
        }
        if (dataTitle == "player_percent" )
        {
            int newPercent;
            player_two_msg.getInt(1) => newPercent;
            if (newPercent != player_two_old_percent)
            {
                Machine.add( "Hit.ck" );
            }
            newPercent => player_two_old_percent;
        }
        if (dataTitle == "player_stock")
        {
            int newStock;
            player_two_msg.getInt(1) => newStock;
            if (player_two_old_stock - 1 == newStock)
            {
                Machine.add( "Death.ck" );
            }
            newStock => player_two_old_stock;
        }
    }
}
