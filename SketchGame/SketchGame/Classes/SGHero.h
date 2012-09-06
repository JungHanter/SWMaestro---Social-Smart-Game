//
//  SGHero.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#ifndef __SketchGame_TouchInput__SGHero__
#define __SketchGame_TouchInput__SGHero__

#include "cocos2d.h"
using namespace cocos2d;

struct SGHeroInfo {
    int Str;
    int Con;
    int Dex;
    int Luck;
    uint64_t ink;
    uint64_t score[10];
};

enum HeroDefendState { DEF_STATE_DEFEND, DEF_STATE_GUARD, DEF_STATE_DODGE };

class SGHero : public CCObject {
public:
    void hideHeroObject();
    void confirmBattleMode();
    void beginBattle();
    void endBattle();
    
    int attack();  //Attack, return atk
    void defend(int defState, int damage);
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();

    void initHeroState(const SGHeroInfo& info);
    
public:
    void func_startHide();
    void func_MoveHide();
    void func_MoveShow();
    void func_startRun();
private:
    int actState;
    int maxHP, nowHP;
    int atk;
    
    CCSprite* heroSprite;
    CCActionInterval* act_attack;
    CCActionInterval* act_run, *act_hide, *act_show, *act_keep, *act_wait, *act_defend, *act_die;


private:
    SGHero(CCLayer* parent);
    static SGHero* sharedSGHero;
    CCLayer* parentLayer;
    
public:
    virtual ~SGHero();
    static SGHero* sharedInstance(CCLayer* parent);
};



#endif /* defined(__SketchGame_TouchInput__SGHero__) */
