#ifndef NAVIGATE_MENU_H
#define NAVIGATE_MENU_H

#include "Goal.h"
#include "../Util/GameState.h"

//We do all the menu logic here so as to not clutter up the lower goal objects space
class NavigateMenu : public Goal
{
public:
    NavigateMenu();
    ~NavigateMenu();

    void Strategize();
    std::string ToString(){return "NavigateMenu";};

private:
    Controller *m_controller;
    bool m_emptiedInput;
    bool m_characterSelected;
};

#endif
