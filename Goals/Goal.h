#ifndef GOAL_H
#define GOAL_H

#include <string>

#include "../Strategies/Strategy.h"
#include "../Util/GameState.h"

//The Goal of the AI represents the highest level objective. What is it trying to do? Win the match?
//  Embarras your opponent? Maybe it's not even supposed to play at all, and just show off tech skill,
//  like during handwarmers.
class Goal
{

public:
    virtual ~Goal(){};
    Goal(){m_state = GameState::Instance();};
    //TODO: Eventually, we're going to set other top level goals. Stuff like "sandbag" or "swag".
    //  For now, let's concentrate on winning the game

    //Determine what the best strategy is, based on the current matchup / config.
    //  Not a whole lot of decisions to be made at this point
    virtual void Strategize() = 0;

    virtual std::string ToString() = 0;

    std::string ToStringCascade(){if(m_strategy==NULL){return ToString()+",NULL,NULL,NULL";}return ToString() + "," + m_strategy->ToStringCascade();};

protected:
    Strategy *m_strategy;
    GameState *m_state;
};

#define CreateStrategy(TYPE) if(m_strategy==NULL){m_strategy = new TYPE();} \
    if(typeid(*m_strategy) != typeid(TYPE)){delete m_strategy;m_strategy = new TYPE();}

#endif
