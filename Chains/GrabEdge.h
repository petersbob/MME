#ifndef GRABEDGE_H
#define GRABEDGE_H

#include "Chain.h"

//Grab the edge from the stage
//NOTE: We are using the "Crouch, turn, wavedash back" method. This is not strictly fastest.
//  But it is probably the most reliable / easiest to make
class GrabEdge : public Chain
{

public:

    GrabEdge();
    ~GrabEdge();

    void PressButtons();
    bool IsInterruptible();
    std::string ToString(){return "GrabEdge";};

private:
    bool m_isLeftEdge;
    bool m_isInWavedash;
    bool m_isInFastfall;
    bool m_moonwalkPrevent;
};

#endif
