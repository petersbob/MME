#ifndef NOTHING_H
#define NOTHING_H

#include "Chain.h"

//Do absolutely nothing
class Nothing : public Chain
{

public:

    Nothing();
    ~Nothing();
    //Determine what buttons to press in order to execute our tactic
    void PressButtons();
    bool IsInterruptible();
    std::string ToString(){return "Nothing";};

private:
    bool m_reset;
};

#endif
