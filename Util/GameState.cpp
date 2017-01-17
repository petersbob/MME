#include <string>
#include <algorithm>
#include <cstdlib>
#include <cmath>

#include <iostream>

#include "GameState.h"
#include "Constants.h"

#include "../osc/OscOutboundPacketStream.h"
#include "../ip/UdpSocket.h"

GameState* GameState::m_instance = NULL;

GameState *GameState::Instance()
{
    if (!m_instance)
    {
        m_instance = new GameState();
    }
    return m_instance;
}

GameState::GameState()
{
    m_memory = new GameMemory();
    m_memory->menu_state = CHARACTER_SELECT; //Let's assume we're in the character select screen unless we know otherwise
    m_rollStartPosition = 0;
    m_edgeInvincibilityStart = 0;
    m_edgeInvincibilityStartSelf = 0;
    m_rollStartSpeed = 0;
    m_rollStartSpeedSelf = 0;
    m_moonwalkRisk = false;
    //Helper booleans to tell us who is on platforms or not
    m_on_platform_self = false;
    m_on_platform_left_self = false;
    m_on_platform_right_self = false;
    m_on_platform_top_self = false;
    m_on_platform_opponent = false;
    m_on_platform_left_opponent = false;
    m_on_platform_right_opponent = false;
    m_on_platform_top_opponent = false;
}

double GameState::getStageEdgePosition()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 71.3078536987;
        }
        case FINAL_DESTINATION:
        {
            return 88.4735488892;
        }
        case DREAMLAND:
        {
            return 80.1791534424;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 66.2554016113;
        }
        case POKEMON_STADIUM:
        {
            return 90.657852;
        }
        case YOSHI_STORY:
        {
            return 58.907848;
        }
    }
    return 1000;
}

double GameState::getStageEdgeGroundPosition()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 68.4000015259;
        }
        case FINAL_DESTINATION:
        {
            return 85.5656967163;
        }
        case DREAMLAND:
        {
            return 77.2713012695;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 63.3475494385;
        }
        case POKEMON_STADIUM:
        {
            return 87.75;
        }
        case YOSHI_STORY:
        {
            return 56;
        }
    }
    return 1000;
}

//Does the current stage have side platforms?
bool GameState::hasSidePlatforms()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        case POKEMON_STADIUM:
        case DREAMLAND:
        case FOUNTAIN_OF_DREAMS:
        case YOSHI_STORY:
        {
            return true;
        }
        case FINAL_DESTINATION:
        {
            return false;
        }
    }
    return false;
}

//Does the current stage have a top platform?
bool GameState::hasTopPlatform()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        case DREAMLAND:
        case FOUNTAIN_OF_DREAMS:
        case YOSHI_STORY:
        {
            return true;
        }
        case POKEMON_STADIUM:
        case FINAL_DESTINATION:
        {
            return false;
        }
    }
    return false;
}

double GameState::sidePlatformHeight()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 27.2001;
        }
        case FINAL_DESTINATION:
        {
            return 1000;
        }
        case DREAMLAND:
        {
            return 30.142199;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 1000; //These platforms move around...
        }
        case POKEMON_STADIUM:
        {
            return 25.000099;
        }
        case YOSHI_STORY:
        {
            return 23.450098;
        }
    }
    return 1000;
}

double GameState::sidePlatformOutterEdge()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 57.600002;
        }
        case FINAL_DESTINATION:
        {
            return 1000;
        }
        case DREAMLAND:
        {
            return 61.392899;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 49.5;
        }
        case POKEMON_STADIUM:
        {
            return 55;
        }
        case YOSHI_STORY:
        {
            return 59.5;
        }
    }
    return 1000;
}

double GameState::sidePlatformInnerEdge()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 20;
        }
        case FINAL_DESTINATION:
        {
            return 1000;
        }
        case DREAMLAND:
        {
            return 31.725401;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 21;
        }
        case POKEMON_STADIUM:
        {
            return 25;
        }
        case YOSHI_STORY:
        {
            return 28;
        }
    }
    return 1000;
}

double GameState::topPlatformHeight()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 54.400101;
        }
        case FINAL_DESTINATION:
        {
            return 1000;
        }
        case DREAMLAND:
        {
            return 51.4254;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 42.750099;
        }
        case POKEMON_STADIUM:
        {
            return 1000;
        }
        case YOSHI_STORY:
        {
            return 42.000099;
        }
    }
    return 1000;
}

double GameState::topPlatformRightEdge()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return 18.800001;
        }
        case FINAL_DESTINATION:
        {
            return 1000;
        }
        case DREAMLAND:
        {
            return 19.017099;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return 14.25;
        }
        case POKEMON_STADIUM:
        {
            return 1000;
        }
        case YOSHI_STORY:
        {
            return 15.75;
        }
    }
    return 1000;
}

double GameState::topPlatformLeftEdge()
{
    switch(m_memory->stage)
    {
        case BATTLEFIELD:
        {
            return -18.800001;
        }
        case FINAL_DESTINATION:
        {
            return 1000;
        }
        case DREAMLAND:
        {
            return -19.018101;
        }
        case FOUNTAIN_OF_DREAMS:
        {
            return -14.25;
        }
        case POKEMON_STADIUM:
        {
            return 1000;
        }
        case YOSHI_STORY:
        {
            return -15.75;
        }
    }
    return 1000;
}


bool GameState::isDamageState(ACTION action)
{
    //Luckily, most of the damage states are contiguous
    if(action >= DAMAGE_HIGH_1 && action <= DAMAGE_FLY_ROLL)
    {
        return true;
    }
    if(action == THROWN_FORWARD ||
        action == THROWN_BACK ||
        action == THROWN_UP ||
        action == THROWN_DOWN ||
        action == DAMAGE_GROUND ||
        action == SLIDING_OFF_EDGE) //Not 100% about this
    {
        return true;
    }

    return false;
}

uint GameState::firstHitboxFrame(CHARACTER character, ACTION action)
{
    switch(character)
    {
        case MARTH:
        {
            switch(action)
            {
                case FSMASH_MID:
                {
                    return 10;
                }
                case DOWNSMASH:
                {
                    return 6;
                }
                case UPSMASH:
                {
                    return 13;
                }
                case DASH_ATTACK:
                {
                    return 12;
                }
                case GRAB:
                {
                    return 7;
                }
                case GRAB_RUNNING:
                {
                    return 10;
                }
                case FTILT_HIGH:
                case FTILT_HIGH_MID:
                case FTILT_MID:
                case FTILT_LOW_MID:
                case FTILT_LOW:
                {
                    return 7;
                }
                case UPTILT:
                {
                    return 7;
                }
                case DOWNTILT:
                {
                    return 7;
                }
                case SWORD_DANCE_1:
                case SWORD_DANCE_1_AIR:
                {
                    return 6;
                }
                case SWORD_DANCE_2_HIGH:
                case SWORD_DANCE_2_HIGH_AIR:
                {
                    return 12;
                }
                case SWORD_DANCE_2_MID:
                case SWORD_DANCE_2_MID_AIR:
                {
                    return 14;
                }
                case SWORD_DANCE_3_HIGH:
                case SWORD_DANCE_3_HIGH_AIR:
                {
                    return 13;
                }
                case SWORD_DANCE_3_MID:
                case SWORD_DANCE_3_MID_AIR:
                {
                    return 11;
                }
                case SWORD_DANCE_3_LOW:
                case SWORD_DANCE_3_LOW_AIR:
                {
                    return 15;
                }
                case SWORD_DANCE_4_HIGH:
                case SWORD_DANCE_4_HIGH_AIR:
                {
                    return 20;
                }
                case SWORD_DANCE_4_MID:
                case SWORD_DANCE_4_MID_AIR:
                {
                    return 23;
                }
                case SWORD_DANCE_4_LOW:
                case SWORD_DANCE_4_LOW_AIR:
                {
                    return 13;
                }
                case UP_B:
                case UP_B_GROUND:
                {
                    return 5;
                }
                case NAIR:
                {
                    return 6;
                }
                case UAIR:
                {
                    return 5;
                }
                case DAIR:
                {
                    return 6;
                }
                case BAIR:
                {
                    return 7;
                }
                case FAIR:
                {
                    return 4;
                }
                case NEUTRAL_ATTACK_1:
                {
                    return 4;
                }
                case NEUTRAL_ATTACK_2:
                {
                    return 6;
                }
                case NEUTRAL_B_ATTACKING:
                case NEUTRAL_B_FULL_CHARGE:
                case NEUTRAL_B_ATTACKING_AIR:
                case NEUTRAL_B_FULL_CHARGE_AIR:
                {
                    return 5;
                }
                case EDGE_ATTACK_SLOW:
                {
                    return 38;
                }
                case EDGE_ATTACK_QUICK:
                {
                    return 25;
                }
                case GROUND_ATTACK_UP:
                {
                    return 20;
                }
                default:
                {
                    return 0;
                    break;
                }
            }
            break;
        }
        default:
        {
            return 0;
            break;
        }
    }
}

uint GameState::lastHitboxFrame(CHARACTER character, ACTION action)
{
    switch(character)
    {
        case MARTH:
        {
            switch(action)
            {
                case FSMASH_MID:
                {
                    return 13;
                }
                case DOWNSMASH:
                {
                    return 23;
                }
                case UPSMASH:
                {
                    return 16;
                }
                case DASH_ATTACK:
                {
                    return 15;
                }
                case GRAB:
                {
                    return 8;
                }
                case GRAB_RUNNING:
                {
                    return 11;
                }
                case FTILT_HIGH:
                case FTILT_HIGH_MID:
                case FTILT_MID:
                case FTILT_LOW_MID:
                case FTILT_LOW:
                {
                    return 10;
                }
                case UPTILT:
                {
                    return 13;
                }
                case DOWNTILT:
                {
                    return 9;
                }
                case SWORD_DANCE_1:
                case SWORD_DANCE_1_AIR:
                {
                    return 8;
                }
                case SWORD_DANCE_2_HIGH:
                case SWORD_DANCE_2_HIGH_AIR:
                {
                    return 15;
                }
                case SWORD_DANCE_2_MID:
                case SWORD_DANCE_2_MID_AIR:
                {
                    return 16;
                }
                case SWORD_DANCE_3_HIGH:
                case SWORD_DANCE_3_HIGH_AIR:
                {
                    return 17;
                }
                case SWORD_DANCE_3_MID:
                case SWORD_DANCE_3_MID_AIR:
                {
                    return 14;
                }
                case SWORD_DANCE_3_LOW:
                case SWORD_DANCE_3_LOW_AIR:
                {
                    return 18;
                }
                case SWORD_DANCE_4_HIGH:
                case SWORD_DANCE_4_HIGH_AIR:
                {
                    return 25;
                }
                case SWORD_DANCE_4_MID:
                case SWORD_DANCE_4_MID_AIR:
                {
                    return 26;
                }
                case SWORD_DANCE_4_LOW:
                case SWORD_DANCE_4_LOW_AIR:
                {
                    return 38;
                }
                case UP_B:
                case UP_B_GROUND:
                {
                    return 10;
                }
                case NAIR:
                {
                    return 21;
                }
                case UAIR:
                {
                    return 8;
                }
                case DAIR:
                {
                    return 9;
                }
                case BAIR:
                {
                    return 11;
                }
                case FAIR:
                {
                    return 7;
                }
                case NEUTRAL_ATTACK_1:
                {
                    return 7;
                }
                case NEUTRAL_ATTACK_2:
                {
                    return 10;
                }
                case NEUTRAL_B_ATTACKING:
                case NEUTRAL_B_FULL_CHARGE:
                case NEUTRAL_B_ATTACKING_AIR:
                case NEUTRAL_B_FULL_CHARGE_AIR:
                {
                    return 10;
                }
                case EDGE_ATTACK_SLOW:
                {
                    return 41;
                }
                case EDGE_ATTACK_QUICK:
                {
                    return 28;
                }
                case GROUND_ATTACK_UP:
                {
                    return 31;
                }
                default:
                {
                    return 0;
                    break;
                }
            }
            break;
        }
        default:
        {
            return 0;
            break;
        }
    }
}


uint GameState::landingLag(CHARACTER character, ACTION action)
{
    switch(character)
    {
        case MARTH:
        {
            switch(action)
            {
                case NAIR:
                {
                    return 7;
                }
                case FAIR:
                {
                    return 7;
                }
                case BAIR:
                {
                    return 12;
                }
                case UAIR:
                {
                    return 7;
                }
                case DAIR:
                {
                    return 16;
                }
                default:
                {
                    return 0;
                }
            }
        }
        default:
        {
            return 0;
        }
    }
}

uint GameState::totalActionFrames(CHARACTER character, ACTION action)
{
    switch(character)
    {
        case MARTH:
        {
            switch(action)
            {
                case FSMASH_MID:
                {
                    return 47;
                }
                case DOWNSMASH:
                {
                    return 61;
                }
                case UPSMASH:
                {
                    return 45;
                }
                case DASH_ATTACK:
                {
                    return 40;
                }
                case GRAB:
                {
                    return 30;
                }
                case GRAB_RUNNING:
                {
                    return 40;
                }
                case FTILT_HIGH:
                case FTILT_HIGH_MID:
                case FTILT_MID:
                case FTILT_LOW_MID:
                case FTILT_LOW:
                {
                    return 35;
                }
                case UPTILT:
                {
                    return 31;
                }
                case DOWNTILT:
                {
                    return 19;
                }
                case SWORD_DANCE_1_AIR:
                case SWORD_DANCE_1:
                {
                    return 29;
                }
                case SWORD_DANCE_2_HIGH_AIR:
                case SWORD_DANCE_2_MID_AIR:
                case SWORD_DANCE_2_HIGH:
                case SWORD_DANCE_2_MID:
                {
                    return 40;
                }
                case SWORD_DANCE_3_HIGH_AIR:
                case SWORD_DANCE_3_MID_AIR:
                case SWORD_DANCE_3_LOW_AIR:
                case SWORD_DANCE_3_HIGH:
                case SWORD_DANCE_3_MID:
                case SWORD_DANCE_3_LOW:
                {
                    return 46;
                }
                case SWORD_DANCE_4_HIGH:
                case SWORD_DANCE_4_MID:
                case SWORD_DANCE_4_HIGH_AIR:
                case SWORD_DANCE_4_MID_AIR:
                {
                    return 50;
                }
                case SWORD_DANCE_4_LOW:
                case SWORD_DANCE_4_LOW_AIR:
                {
                    return 60;
                }
                case UP_B:
                case UP_B_GROUND:
                {
                    return 10;
                }
                case NAIR:
                {
                    return 21;
                }
                case UAIR:
                {
                    return 25;
                }
                case DAIR:
                {
                    return 48;
                }
                case BAIR:
                {
                    return 32;
                }
                case FAIR:
                {
                    return 27;
                }
                case NEUTRAL_ATTACK_1:
                {
                    return 19;
                }
                case NEUTRAL_ATTACK_2:
                {
                    return 20;
                }
                case NEUTRAL_B_ATTACKING:
                case NEUTRAL_B_FULL_CHARGE:
                case NEUTRAL_B_ATTACKING_AIR:
                case NEUTRAL_B_FULL_CHARGE_AIR:
                {
                    return 33;
                }
                case EDGE_ATTACK_SLOW:
                {
                    return 68;
                }
                case EDGE_ATTACK_QUICK:
                {
                    return 54;
                }
                case LANDING_SPECIAL:
                {
                    return 30;
                }
                case MARTH_COUNTER_FALLING:
                case MARTH_COUNTER:
                {
                    return 59;
                }
                case SPOTDODGE:
                {
                    return 27;
                }
                case ROLL_FORWARD:
                case ROLL_BACKWARD:
                {
                    return 35;
                }
                case EDGE_ROLL_SLOW:
                {
                    return 98;
                }
                case EDGE_ROLL_QUICK:
                {
                    return 48;
                }
                case EDGE_GETUP_SLOW:
                {
                    return 58;
                }
                case EDGE_GETUP_QUICK:
                {
                    return 32;
                }
                case NEUTRAL_GETUP:
                {
                    return 30;
                }
                case NEUTRAL_TECH:
                {
                    return 26;
                }
                case FORWARD_TECH:
                {
                    return 40;
                }
                case BACKWARD_TECH:
                {
                    return 40;
                }
                case GETUP_ATTACK:
                {
                    return 23;
                }
                case TECH_MISS_UP:
                {
                    return 26;
                }
                case TECH_MISS_DOWN:
                {
                    return 26;
                }
                case GROUND_ROLL_BACKWARD_UP:
                case GROUND_ROLL_FORWARD_UP:
                case GROUND_ROLL_FORWARD_DOWN:
                case GROUND_ROLL_BACKWARD_DOWN:
                {
                    return 35;
                }
                case GROUND_GETUP:
                {
                    return 30;
                }
                case GROUND_ATTACK_UP:
                {
                    return 49;
                }
                case WAVEDASH_SLIDE:
                {
                    return 10;
                }
                default:
                {
                    return 0;
                    break;
                }
            }
            break;
        }
        case FOX:
        {
            switch(action)
            {
                case THROW_DOWN:
                {
                    return 43;
                }
                case LANDING_SPECIAL:
                {
                    return 30;
                }
                case WAVEDASH_SLIDE:
                {
                    return 10;
                }
                default:
                {
                    return 0;
                }
            }
        }
        default:
        {
            return 0;
            break;
        }
    }
}

bool GameState::isAttacking(ACTION action)
{
    switch(action)
    {
        case FSMASH_MID:
        case DOWNSMASH:
        case UPSMASH:
        case DASH_ATTACK:
        case GRAB:
        case GRAB_RUNNING:
        case FTILT_HIGH:
        case FTILT_HIGH_MID:
        case FTILT_MID:
        case FTILT_LOW_MID:
        case FTILT_LOW:
        case UPTILT:
        case DOWNTILT:
        case SWORD_DANCE_1:
        case SWORD_DANCE_2_HIGH:
        case SWORD_DANCE_2_MID:
        case SWORD_DANCE_3_HIGH:
        case SWORD_DANCE_3_MID:
        case SWORD_DANCE_3_LOW:
        case SWORD_DANCE_4_HIGH:
        case SWORD_DANCE_4_MID:
        case SWORD_DANCE_4_LOW:
        case SWORD_DANCE_1_AIR:
        case SWORD_DANCE_2_HIGH_AIR:
        case SWORD_DANCE_2_MID_AIR:
        case SWORD_DANCE_3_HIGH_AIR:
        case SWORD_DANCE_3_MID_AIR:
        case SWORD_DANCE_3_LOW_AIR:
        case SWORD_DANCE_4_HIGH_AIR:
        case SWORD_DANCE_4_MID_AIR:
        case SWORD_DANCE_4_LOW_AIR:
        case UP_B:
        case UP_B_GROUND:
        case NAIR:
        case UAIR:
        case DAIR:
        case BAIR:
        case FAIR:
        case NEUTRAL_ATTACK_1:
        case NEUTRAL_ATTACK_2:
        case NEUTRAL_ATTACK_3:
        case NEUTRAL_B_ATTACKING:
        case NEUTRAL_B_FULL_CHARGE:
        case NEUTRAL_B_ATTACKING_AIR:
        case NEUTRAL_B_FULL_CHARGE_AIR:
        case EDGE_ATTACK_QUICK:
        case EDGE_ATTACK_SLOW:
        case GROUND_ATTACK_UP:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool GameState::isReverseHit(ACTION action)
{
    switch(action)
    {
        case DOWNSMASH:
        case UPSMASH:
        case GRAB_RUNNING:
        case UPTILT:
        case NAIR:
        case UAIR:
        case DAIR:
        case BAIR:
        case GROUND_ATTACK_UP:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool GameState::isIndexedFromZero(ACTION action)
{
    switch(action)
    {
        case DEAD_FLY_STAR:
        case DEAD_FLY:
        case DEAD_FLY_SPLATTER:
        case ON_HALO_DESCENT:
        case ON_HALO_WAIT:
        case FALLING_AERIAL:
        case CROUCH_END:
        case NEUTRAL_TECH:
        case FORWARD_TECH:
        case BACKWARD_TECH:
        case LANDING_SPECIAL:
        case WALK_MIDDLE:
        case WALK_FAST:
        case DEAD_FALL:
        case RUNNING:
        case JUMPING_FORWARD:
        case JUMPING_BACKWARD:
        case JUMPING_ARIAL_FORWARD:
        case JUMPING_ARIAL_BACKWARD:
        case TUMBLING:
        case NEUTRAL_ATTACK_2:
        case EDGE_TEETERING_START:
        case EDGE_TEETERING:
        case EDGE_HANGING:
        case GRABBED:
        case GRAB:
        case GRAB_WAIT:
        case GRABBED_WAIT_HIGH:
        case PUMMELED_HIGH:
        case GRAB_PUMMEL:
        case GRAB_PUMMELED:
        case GRAB_RUNNING:
        case KNEE_BEND:
        case SHIELD_RELEASE:
        case SHIELD_STUN:
        case STANDING:
        case SLIDING_OFF_EDGE:
        case TECH_MISS_UP:
        case ENTRY_START:
        case UAIR_LANDING:
        case BAIR_LANDING:
        case DAIR_LANDING:
        case FALLING:
        case LANDING:
        case WAIT_ITEM:
        case NEUTRAL_B_CHARGING_AIR:
        case DOWN_B_GROUND:
        case DOWN_B_AIR:
        case SWORD_DANCE_4_HIGH_AIR:
        case SWORD_DANCE_3_LOW_AIR:
        case FIREFOX_AIR:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool GameState::isRollingState(ACTION action)
{
    switch(action)
    {
        case ROLL_FORWARD:
        case ROLL_BACKWARD:
        case SPOTDODGE:
        case EDGE_ROLL_SLOW:
        case EDGE_ROLL_QUICK:
        case EDGE_GETUP_SLOW:
        case EDGE_GETUP_QUICK:
        case NEUTRAL_GETUP:
        case NEUTRAL_TECH:
        case FORWARD_TECH:
        case BACKWARD_TECH:
        case MARTH_COUNTER_FALLING:
        case MARTH_COUNTER:
        case TECH_MISS_UP:
        case TECH_MISS_DOWN:
        case GROUND_ROLL_BACKWARD_UP:
        case GROUND_ROLL_FORWARD_UP:
        case GROUND_ROLL_FORWARD_DOWN:
        case GROUND_ROLL_BACKWARD_DOWN:
        case GROUND_GETUP:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool GameState::isGrabbedState(ACTION action)
{
    switch(action)
    {
        case GRAB_PULL:
        case GRABBED:
        case SPOTDODGE:
        case GRAB_PUMMELED:
        case GRAB_ESCAPE:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

double GameState::calculateSlideDistance(CHARACTER character, double initSpeed, int frames)
{
    double slideCoeficient;

    switch (character)
    {
        case FOX:
        {
            slideCoeficient = FOX_SLIDE_COEFICIENT;
            break;
        }
        case MARTH:
        {
            if(m_memory->player_one_action == TECH_MISS_UP ||
                m_memory->player_one_action == TECH_MISS_DOWN)
            {
                slideCoeficient = 0.05;
            }
            else
            {
                slideCoeficient = MARTH_SLIDE_COEFICIENT;
            }
            break;
        }
        default:
        {
            //Probably good enough until all characters are supported
            slideCoeficient = 0.05;
        }
    }

    //This determines magnitude of slide
    double slideDistance = 0;
    for(int i = 1; i <= frames; i++)
    {
        slideDistance += std::max(std::abs(initSpeed) - (i * slideCoeficient), 0.0);
    }

    //Determine direction
    if(initSpeed < 0)
    {
        return (-1.0) * slideDistance;
    }
    return slideDistance;
}

double GameState::getRollDistance(CHARACTER character, ACTION action)
{
    switch (character)
    {
        case MARTH:
        {
            switch(action)
            {
                case ROLL_FORWARD:
                case ROLL_BACKWARD:
                {
                    return 38.95;
                }
                case EDGE_ROLL_SLOW:
                case EDGE_ROLL_QUICK:
                {
                    return 41.44;
                }
                case EDGE_GETUP_SLOW:
                case EDGE_GETUP_QUICK:
                {
                    return 11.33;
                }
                case FORWARD_TECH:
                case BACKWARD_TECH:
                {
                    return 46.711546;//new
                }
                case GROUND_ROLL_FORWARD_UP:
                {
                    return 36.67;
                }
                case GROUND_ROLL_BACKWARD_UP:
                {
                    return 35.83;
                }
                case GROUND_ROLL_FORWARD_DOWN:
                {
                    return 34.65;
                }
                case GROUND_ROLL_BACKWARD_DOWN:
                {
                    return 34.66;
                }
                case NEUTRAL_TECH:
                {
                    return 5.93;
                }
                default:
                {
                    return 0;
                }
            }
        }
        case FOX:
        {
            switch(action)
            {
                case ROLL_FORWARD:
                case ROLL_BACKWARD:
                {
                    return 33.6;
                }
                default:
                {
                    return 0;
                }
            }
        }
        default:
        {
            return 0;
        }
    }
}

bool GameState::getRollDirection(ACTION action)
{
    switch(action)
    {
        case ROLL_FORWARD:
        case EDGE_ROLL_SLOW:
        case EDGE_ROLL_QUICK:
        case EDGE_GETUP_SLOW:
        case EDGE_GETUP_QUICK:
        case FORWARD_TECH:
        case GROUND_ROLL_FORWARD_UP:
        case GROUND_ROLL_FORWARD_DOWN:
        {
            return true;
        }
        case ROLL_BACKWARD:
        case BACKWARD_TECH:
        case GROUND_ROLL_BACKWARD_UP:
        case GROUND_ROLL_BACKWARD_DOWN:
        case NEUTRAL_TECH:
        {
            return false;
        }
        default:
        {
            return true;
        }
    }
}

uint GameState::trailingVulnerableFrames(CHARACTER character, ACTION action)
{
    switch (character)
    {
        case MARTH:
        {
            switch(action)
            {
                case EDGE_GETUP_QUICK:
                {
                    return 2;
                }
                case EDGE_GETUP_SLOW:
                {
                    return 3;
                }
                case MARTH_COUNTER:
                {
                    return 29;
                }
                case GROUND_ROLL_BACKWARD_DOWN:
                {
                    return 5; //TODO: this is a guess
                }
                case GROUND_ATTACK_UP:
                {
                    return 5;
                }
                default:
                {
                    return 7;
                }
            }
        }
        default:
        {
            return 0;
        }
    }
}

bool GameState::hasMultipleHitboxes(CHARACTER character, ACTION action)
{
    switch(character)
    {
        case MARTH:
        {
            switch(action)
            {
                case SWORD_DANCE_4_LOW:
                case SWORD_DANCE_4_LOW_AIR:
                case GROUND_ATTACK_UP:
                {
                    return true;
                }
                default:
                {
                    return false;
                }
            }
            break;
        }
        default:
        {
            return false;
        }
    }
}

double GameState::getMaxHorizontalAirSpeed(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 0.88875;
        }
        case FOX:
        {
            return 0.819625;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getInitHorizontalAirSpeed(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 0.9825;
        }
        case FOX:
        {
            return 0.86875;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getInitVerticalAirSpeed(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 2.027;
        }
        case FOX:
        {
            return 4.186;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getHorizontalAirAccel(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 0.005;
        }
        case FOX:
        {
            return 0.02;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::calculateMaxAirDistance(CHARACTER character, double initSpeed, int frames, bool direction)
{
    double accel = getHorizontalAirAccel(character);
    double maxSpeed = getMaxHorizontalAirSpeed(character);
    double negMaxSpeed = (-1.0)*maxSpeed;

    //This determines magnitude of slide
    double totalDistance = 0;

    for(int i = 1; i <= frames; i++)
    {
        //If going right...
        if(direction)
        {
            if(initSpeed > maxSpeed)
            {
                //Decelerate down to max speed
                totalDistance += std::max(initSpeed - (i * accel), maxSpeed);
            }
            else
            {
                //Accelerate up to max speed
                totalDistance += std::min(initSpeed + (i * accel), maxSpeed);
            }
        }
        else
        {
            if(initSpeed < negMaxSpeed)
            {
                //Decelerate down to max speed
                totalDistance += std::min(initSpeed + (i * accel), negMaxSpeed);
            }
            else
            {
                //Accelerate up to max speed
                totalDistance += std::max(initSpeed - (i * accel), negMaxSpeed);
            }
        }
    }
    return totalDistance;
}

int GameState::getFramesUntilFallingFromJump(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 24;
        }
        case FOX:
        {
            return 20;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getDoubleJumpHeightMax(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 25.187989;
        }
        case FOX:
        {
            return 40.203998;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getGravity(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 0.085;
        }
        case FOX:
        {
            return 0.23;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getMaxFallingSpeed(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 2.2;
        }
        case FOX:
        {
            return 2.8;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::getFastfallSpeed(CHARACTER character)
{
    switch(character)
    {
        case MARTH:
        {
            return 2.5;
        }
        case FOX:
        {
            return 3.4;
        }
        default:
        {
            return 0;
        }
    }
}

double GameState::calculateDoubleJumpHeight(CHARACTER character, double initSpeed)
{
    double gravity = getGravity(character);
    double height = 0;

    for(int i = 1;; i++)
    {
        double tick = initSpeed - (i * gravity);
        if(tick < 0 || i > 100)
        {
            break;
        }
        height += tick;
    }
    return height;
}

bool GameState::shareData(GameMemory* gm)
{

  UdpTransmitSocket transmitSocket( IpEndpointName( ADDRESS, PORT ) );

  char buffer[OUTPUT_BUFFER_SIZE];
  osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

  p << osc::BeginBundleImmediate
  //player one messages
    << osc::BeginMessage( "/player_one_info" )
        << "player_y" << (float)gm->player_one_y << osc::EndMessage
    << osc::BeginMessage( "/player_one_info" )
        << "player_percent" << (int)gm->player_one_percent << osc::EndMessage
    << osc::BeginMessage( "/player_one_info" )
        << "player_stock" << (int)gm->player_one_stock << osc::EndMessage
    << osc::BeginMessage( "/player_one_info" )
        << "player_jumps" << (int)gm->player_one_jumps_left << osc::EndMessage
    //player two messages
        << osc::BeginMessage( "/player_two_info" )
        << "player_y" << (float)gm->player_two_y << osc::EndMessage
    << osc::BeginMessage( "/player_two_info" )
        << "player_percent" << (int)gm->player_two_percent << osc::EndMessage
    << osc::BeginMessage( "/player_two_info" )
        << "player_stock" << (int)gm->player_two_stock << osc::EndMessage
    << osc::EndBundle;


  transmitSocket.Send( p.Data(), p.Size() );
  return true;
}
