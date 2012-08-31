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
    unsigned int Str;
    unsigned int Con;
    unsigned int Dex;
    unsigned int Luck;
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
    
    void attack();  //공격
    void defend(int defState);  //맞을때 (수비하던지 회피하던지 하겠지)
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();
    
private:
    void func_startHide();
    void func_MoveHide();
    void func_MoveShow();
    void func_startRun();
    
private:
    int type;
    int actState;
    int maxHP, nowHP;
    int atk;
    
    CCSprite* heroSprite;
    CCActionInterval** act_attack;
    CCActionInterval* act_run, *act_hide, *act_wait, *act_defend, *act_die;

//싱글톤패턴
private:
    SGHero();
    static SGHero* hero;
    
public:
    ~SGHero();
    static SGHero* sharedInstance();
    static void initWithHeroInfo(SGHeroInfo& info);
};

SGHero* SGHero::hero = NULL;

#endif /* defined(__SketchGame_TouchInput__SGHero__) */
