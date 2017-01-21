#include <typeinfo>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>

#include "Recover.h"
#include "../Util/Constants.h"
#include "../Chains/Nothing.h"
#include "../Chains/EdgeAction.h"
#include "../Chains/FireFox.h"
#include "../Chains/FullJump.h"
#include "../Chains/DI.h"
#include "../Chains/Illusion.h"
#include "../Util/Logger.h"

Recover::Recover()
{
    m_chain = NULL;
}

Recover::~Recover()
{
    delete m_chain;
}

void Recover::DetermineChain()
{
    //If we're not in a state to interupt, just continue with what we've got going
    if((m_chain != NULL) && (!m_chain->IsInterruptible()))
    {
        m_chain->PressButtons();
        return;
    }

    //If we're hanging on the egde, wavedash on
    if(m_state->m_memory->player_two_action == EDGE_HANGING ||
        m_state->m_memory->player_two_action == EDGE_CATCHING)
    {
        CreateChain2(EdgeAction, WAVEDASH_UP);
        m_chain->PressButtons();
        return;
    }

    bool selfOnStage = m_state->m_memory->player_two_on_ground ||
        (std::abs(m_state->m_memory->player_two_x) < m_state->getStageEdgeGroundPosition() &&
        m_state->m_memory->player_two_y > -5);

    //If we're off the stage...
    if(!selfOnStage)
    {
        double xDistanceToEdge = std::abs(std::abs(m_state->m_memory->player_two_x) - m_state->getStageEdgePosition());
        bool onRight = m_state->m_memory->player_two_x > 0;
        bool acceptableFallState = m_state->m_memory->player_two_action == FALLING ||
            m_state->m_memory->player_two_action == FALLING_AERIAL ||
            m_state->m_memory->player_two_action == FALLING_AERIAL ||
            m_state->m_memory->player_two_action == DOWN_B_AIR;

        // //If we're in our shine and ABOVE the stage, just let go and fall.
        // if(m_state->m_memory->player_two_action == DOWN_B_AIR &&
        //     m_state->m_memory->player_two_action_frame < 11 &&
        //     m_state->m_memory->player_two_y > -10)
        // {
        //     CreateChain(Nothing);
        //     m_chain->PressButtons();
        //     return;
        // }

        //Can we just fall and grab the edge?
        if(acceptableFallState &&
            m_state->m_memory->player_one_action != EDGE_HANGING &&
            m_state->m_memory->player_one_action != EDGE_CATCHING &&
            xDistanceToEdge < 6.5 &&
            m_state->m_memory->player_two_y > -9 &&
            m_state->m_memory->player_two_speed_y_self <= 0)
        {
            if(m_state->m_memory->player_two_facing != onRight)
            {
                CreateChain(Nothing);
                m_chain->PressButtons();
                return;
            }
            else
            {
                if(m_state->m_memory->player_two_y < -6 &&
                    m_state->m_memory->player_two_action != FOX_ILLUSION &&
                    m_state->m_memory->player_one_action != EDGE_HANGING &&
                    m_state->m_memory->player_one_action != EDGE_CATCHING)
                {
                    //Illusion
                    CreateChain2(Illusion, !onRight);
                    m_chain->PressButtons();
                    return;
                }
                else
                {
                    CreateChain(Nothing);
                    m_chain->PressButtons();
                    return;
                }
            }
        }

        if(m_state->m_memory->player_two_y < -4 &&
            m_state->m_memory->player_two_y > -8 &&
            m_state->m_memory->player_two_action != FOX_ILLUSION_START &&
            m_state->m_memory->player_one_action != EDGE_HANGING &&
            m_state->m_memory->player_one_action != EDGE_CATCHING)
        {
            //Illusion
            CreateChain2(Illusion, !onRight);
            m_chain->PressButtons();
            return;
        }

        //Can we grab the edge, but are moving upwards?
        if(xDistanceToEdge < 4.5 &&
            m_state->m_memory->player_two_y > -15 &&
            m_state->m_memory->player_two_facing != onRight &&
            m_state->m_memory->player_two_speed_y_self > 0)
        {
            CreateChain2(FireFox, true);
            m_chain->PressButtons();
            return;
        }

        //Do we still have a jump. If so, jump
        if(m_state->m_memory->player_two_jumps_left > 0)
        {
            CreateChain(FullJump);
            m_chain->PressButtons();
            return;
        }

        //If we're jumping, just keep jumping until we're above the stage
        if((m_state->m_memory->player_two_action == JUMPING_ARIAL_FORWARD ||
            m_state->m_memory->player_two_action == JUMPING_ARIAL_BACKWARD) &&
            m_state->m_memory->player_two_speed_y_self > 0 &&
            m_state->m_memory->player_two_y < 0)
        {
            CreateChain3(DI, onRight ? 0 : 1, .5);
            m_chain->PressButtons();
            return;
        }
    }

    bool enemyOnStage = m_state->m_memory->player_one_on_ground ||
        (std::abs(m_state->m_memory->player_one_x) < m_state->getStageEdgeGroundPosition() &&
        m_state->m_memory->player_one_y > -5);

    //Firefox back, randomly choose to sweetspot
    bool sweetspot = rand() % 2 == 0;
    if(!enemyOnStage)
    {
        sweetspot = true;
    }
    CreateChain2(FireFox, sweetspot);
    m_chain->PressButtons();
    return;
}
