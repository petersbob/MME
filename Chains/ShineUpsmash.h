#ifndef SHINEUPSMASH_H
#define SHINEUPSMASH_H

#include "Chain.h"

//Jump cancel shine into upsmash
class ShineUpsmash : public Chain
{

public:

    ShineUpsmash();
    ~ShineUpsmash();
    //Determine what buttons to press in order to execute our tactic
    void PressButtons();
    bool IsInterruptible();
    std::string ToString(){return "ShineUpsmash";};

};

#endif
