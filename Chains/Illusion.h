#ifndef ILLUSION_H
#define ILLUSION_H

#include "Chain.h"

class Illusion : public Chain
{

public:

    Illusion(bool goRight);
    ~Illusion();
    //Determine what buttons to press in order to execute our tactic
    void PressButtons();
    bool IsInterruptible();
    std::string ToString(){return "Illusion";};

private:
    bool m_goRight;

};

#endif
