int id;
int player_one_old_stock;
int player_one_old_percent;
int player_two_old_stock;
int player_two_old_percent;

float height;

SinOsc a => dac;
.05 => a.gain;

OscMsg player_one_msg;
OscMsg player_two_msg;

int hitLagFrames;
// Make a receiver, set port#, set up to listen for event
OscIn player_one_oin;
7000 => player_one_oin.port;

// create an address in the receiver, store in new variable
player_one_oin.addAddress( "/player_one_info" );

OscIn player_two_oin;
8000 => player_two_oin.port;

// create an address in the receiver, store in new variable
player_two_oin.addAddress( "/player_two_info" );

// Infinite loop to wait for messages and play notes
while (true)
{
    // OSC message is an event, chuck it to now
    player_one_oin => now;
    // when event(s) received, process them
    while (player_one_oin.recv(player_one_msg) != 0)
    {
        <<< "player_one getting message" >>>;
      // peel off integer, float, string
        player_one_msg.getString(0) => string dataTitle;
        if (dataTitle == "player_y")
        {
            player_one_msg.getFloat(1) => height;
            height + 400 => a.freq;
        }
        if (dataTitle == "player_percent" )
        {
            int newPercent;
            player_one_msg.getInt(1) => newPercent; 
            if (newPercent != player_one_old_percent)
            {
                Machine.add( "Hit.ck" );
            }  
            newPercent => player_one_old_percent;
        }
        if (dataTitle == "player_stock")
        {
            int newStock;
            player_one_msg.getInt(1) => newStock;
            if (player_one_old_stock - 1 == newStock) 
            {
                Machine.add( "Death.ck" );
            }
            newStock => player_one_old_stock;
        }
        
    }

    //player_two_oin => now;
    // when event(s) received, process them
    /*while (player_two_oin.recv(player_two_msg) != 0)
    {
        <<< "player_two getting message" >>>;
        // peel off integer, float, string
        player_two_msg.getString(0) => string dataTitle;
        if (dataTitle == "player_y")
        {
            player_two_msg.getFloat(1) => height;
            height + 400 => a.freq;
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
    */

}
