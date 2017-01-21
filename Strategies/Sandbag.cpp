#include <cmath>
#include <cstdlib>
#include <math.h>

#include "Sandbag.h"
#include "../Util/Constants.h"
#include "../Tactics/Approach.h"
#include "../Tactics/CreateDistance.h"
#include "../Tactics/KeepDistance.h"
#include "../Tactics/Wait.h"
#include "../Tactics/Parry.h"
#include "../Tactics/Recover.h"
#include "../Tactics/ShowOff.h"
#include "../Tactics/Escape.h"
#include "../Util/Logger.h"

Sandbag::Sandbag()
{
    m_tactic = NULL;
    m_shieldedAttack = false;
    m_actionChanged = true;
    m_chargingLastFrame = false;
}

Sandbag::~Sandbag()
{
    delete m_tactic;
}

void Sandbag::DetermineTactic()
{
    //Has opponent just released a charged smash attack?
    bool chargedSmashReleased = false;
    if(!m_state->m_memory->player_one_charging_smash && m_chargingLastFrame)
    {
        chargedSmashReleased = true;
    }
    m_chargingLastFrame = m_state->m_memory->player_one_charging_smash;

    //Update the attack frame if the enemy started a new action
    if(m_state->m_memory->player_one_action_frame == 1)
    {
        m_shieldedAttack = false;
        m_actionChanged = true;
    }
    //Continuing same previous action
    else
    {
        m_actionChanged = false;
        if(m_state->m_memory->player_two_action == SHIELD_STUN ||
            m_state->m_memory->player_two_action == SPOTDODGE ||
            m_state->m_memory->player_two_action == EDGE_GETUP_QUICK ||
            m_state->m_memory->player_two_action == EDGE_GETUP_SLOW)
        {
            m_shieldedAttack = true;
        }
    }

    //If we're not in a state to interupt, just continue with what we've got going
    if((m_tactic != NULL) && (!m_tactic->IsInterruptible()))
    {
        m_tactic->DetermineChain();
        return;
    }

    //If we're still warping in at the start of the match or in shield stun, then just hang out and do nothing
    if(m_state->m_memory->player_two_action == ENTRY ||
        m_state->m_memory->player_two_action == ENTRY_START ||
        m_state->m_memory->player_two_action == ENTRY_END ||
        m_state->m_memory->player_two_action == SHIELD_STUN ||
        m_state->m_memory->player_two_action == WAVEDASH_SLIDE ||
        m_state->m_memory->player_two_action == LANDING_SPECIAL)
    {
        CreateTactic(Wait);
        m_tactic->DetermineChain();
        return;
    }

    //If we're in shield release, but can't punish safely, then just get out of there
    //NOTE: Do not put any punish tactics below here.
    if(m_state->m_memory->player_two_action == SHIELD_RELEASE)
    {
        CreateTactic(CreateDistance);
        m_tactic->DetermineChain();
        return;
    }

    //Calculate distance between players
    double distance = pow(m_state->m_memory->player_one_x - m_state->m_memory->player_two_x, 2);
    distance += pow(m_state->m_memory->player_one_y - m_state->m_memory->player_two_y, 2);
    distance = sqrt(distance);

    //If we're hanging on the egde, and the oponnent is on the stage area, then recover
    if((m_state->m_memory->player_two_action == EDGE_HANGING ||
        m_state->m_memory->player_two_action == EDGE_CATCHING) &&
        std::abs(m_state->m_memory->player_one_x) < m_state->getStageEdgeGroundPosition() + .001 &&
        m_state->m_memory->player_one_y > -5)
    {
        CreateTactic(Recover);
        m_tactic->DetermineChain();
        return;
    }

    //If the enemy is in a looping attack outside our range, back off
    if(distance > 35 &&
        distance < 60 &&
        (m_state->m_memory->player_one_action == DOWNTILT ||
        m_state->m_memory->player_one_action == NEUTRAL_ATTACK_1 ||
        m_state->m_memory->player_one_action == NEUTRAL_ATTACK_2))
    {
        CreateTactic(CreateDistance);
        m_tactic->DetermineChain();
        return;
    }

    //Escape out of our opponents combo / grab if they somehow get it
    if(m_state->isDamageState((ACTION)m_state->m_memory->player_two_action) ||
        m_state->isGrabbedState((ACTION)m_state->m_memory->player_two_action))
    {
        CreateTactic(Escape);
        m_tactic->DetermineChain();
        return;
    }

    //If we need to defend against an attack, that's next priority. Unless we've already shielded this attack
    if(!m_shieldedAttack && distance < MARTH_FSMASH_RANGE)
    {
        //Don't bother parrying if the attack is over
        if(m_state->lastHitboxFrame((CHARACTER)m_state->m_memory->player_one_character, (ACTION)m_state->m_memory->player_one_action) >=
            m_state->m_memory->player_one_action_frame)
        {
            //Don't bother parrying if the attack is in the wrong direction
            bool player_one_is_to_the_left = (m_state->m_memory->player_one_x - m_state->m_memory->player_two_x > 0);
            if((m_state->m_memory->player_one_facing != player_one_is_to_the_left || (m_state->isReverseHit((ACTION)m_state->m_memory->player_one_action))) &&
                (m_state->m_memory->player_two_on_ground ||
                m_state->m_memory->player_two_action == EDGE_HANGING ||
                m_state->m_memory->player_two_action == EDGE_CATCHING))
            {
                if(m_state->isAttacking((ACTION)m_state->m_memory->player_one_action))
                {
                    //If the p1 action changed, scrap the old Parry and make a new one.
                    if(m_actionChanged || chargedSmashReleased)
                    {
                        delete m_tactic;
                        m_tactic = NULL;
                    }

                    CreateTactic(Parry);
                    m_tactic->DetermineChain();
                    return;
                }
            }
        }
    }

    //If the enemy is dead, then celebrate!
    if(m_state->m_memory->player_one_y < MARTH_LOWER_EVENT_HORIZON ||
        (m_state->m_memory->player_one_action >= DEAD_DOWN &&
        m_state->m_memory->player_one_action <= DEAD_FLY_SPLATTER_FLAT) ||
        (m_state->m_memory->player_one_action == DEAD_FALL &&
         m_state->m_memory->player_one_y < -20))
    {
        //Have to be on the ground or on edge
        if(m_state->m_memory->player_two_on_ground ||
            m_state->m_memory->player_two_action == EDGE_HANGING ||
            m_state->m_memory->player_two_action == EDGE_CATCHING ||
            m_state->m_memory->player_two_action == DOWN_B_STUN ||
            m_state->m_memory->player_two_action == DOWN_B_AIR)
        {
            CreateTactic(ShowOff);
            m_tactic->DetermineChain();
            return;
        }
    }

    //If we're off the stage and don't need to edgeguard, get back on
    if(std::abs(m_state->m_memory->player_two_x) > m_state->getStageEdgeGroundPosition() + .001)
    {
        CreateTactic(Recover);
        m_tactic->DetermineChain();
        return;
    }

    //If our opponent is doing something to put them in a vulnerable spot, approach
    if(m_state->m_memory->player_one_action == KNEE_BEND ||
        m_state->m_memory->player_one_action == JUMPING_FORWARD ||
        m_state->m_memory->player_one_action == SHIELD ||
        m_state->m_memory->player_one_action == SHIELD_START ||
        m_state->m_memory->player_one_action == SHIELD_REFLECT ||
        m_state->isDamageState((ACTION)m_state->m_memory->player_one_action))
    {
        CreateTactic2(Approach, true);
        m_tactic->DetermineChain();
        return;
    }

    bool onRight = m_state->m_memory->player_one_x < m_state->m_memory->player_two_x;

    //If our opponent is dashing toward us, approach
    if(m_state->m_memory->player_one_action == DASHING &&
        m_state->m_memory->player_one_facing == onRight)
    {
        CreateTactic2(Approach, true);
        m_tactic->DetermineChain();
        return;
    }

    CreateTactic(KeepDistance);
    m_tactic->DetermineChain();
    return;
}
