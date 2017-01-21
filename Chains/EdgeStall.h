#ifndef EDGESTALL_H
#define EDGESTALL_H

#include "Chain.h"

//Do a fully invincible edge stall
class EdgeStall : public Chain
{

public:

    EdgeStall();
    ~EdgeStall();
    //Determine what buttons to press in order to execute our tactic
    void PressButtons();
    bool IsInterruptible();
    std::string ToString(){return "EdgeStall";};

private:
    bool m_isLeftEdge;
    bool m_pressedBack;
};

#endif
