fun void boing(float y_position)
{
    SinOsc osc => ADSR e => dac;
    SinOsc lfo;

    e.set(100::ms, 100::ms, .3, 500::ms);

    200.0 + y_position * 5 => float freq;
    0.4 => float gain;
    float gain_scale;
    float amp_mod;
    15.0 => lfo.freq;
    lfo => blackhole;

    float theValue;
    for (0 => int i; i < 800; i++)
    {
        freq + 1.0 => freq;
        freq => osc.freq;
        if (gain > .01)
        {
        gain - .001 => gain;
        gain => osc.gain;
        }

        lfo.last() => amp_mod;
        (amp_mod + 1) / 2 => amp_mod;
        amp_mod => gain_scale;
        gain_scale * gain => osc.gain;

        e.keyOn();
        ms => now;
        e.keyOff();
    }
}
SinOsc a => JCRev r => dac;
.5 => r.mix;

//variables used with every frame update
int player_one_old_stock;
int player_one_old_percent;
int player_one_old_jumps;
float player_one_height;

OscMsg player_one_msg;

// Make a receiver, set port#, set up to listen for event
OscIn player_one_oin;
7000 => player_one_oin.port;

// create an address in the receiver, store in new variable
player_one_oin.addAddress( "/player_one_info" );

// Infinite loop to wait for messages and play notes
while (true)
{
    // OSC message is an event, chuck it to now
    player_one_oin => now;
    // when event(s) received, process them
    while (player_one_oin.recv(player_one_msg) != 0)
    {
        // peel off integer, float, string
        player_one_msg.getString(0) => string dataTitle;
        if (dataTitle == "player_y")
        {
            player_one_msg.getFloat(1) => player_one_height;
            if (player_one_height > 5)
            {
              .1 => a.gain;
              player_one_height + 400 => a.freq;
            }
            else
            {
              0 => a.gain;
            }
        }
        if (dataTitle == "player_percent" )
        {
            int newPercent;
            player_one_msg.getInt(1) => newPercent;
            if (newPercent != player_one_old_percent)
            {
                Machine.add( "Chuck/Hit.ck" );
            }
            newPercent => player_one_old_percent;
        }
        if (dataTitle == "player_stock")
        {
            int newStock;
            player_one_msg.getInt(1) => newStock;
            if (player_one_old_stock - 1 == newStock)
            {
                Machine.add( "Chuck/Death.ck" );
            }
            newStock => player_one_old_stock;
        }
        if (dataTitle == "player_jumps")
        {
            int new_jumps;
            player_one_msg.getInt(1) => new_jumps;
            if (new_jumps != player_one_old_jumps && new_jumps <2)
            {
                spork ~ boing(player_one_height);
            }
            new_jumps => player_one_old_jumps;
        }
    }
}
