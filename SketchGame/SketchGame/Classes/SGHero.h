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
    
    int attack();  //공격. return 공격력
    void defend(int defState, int damage);  //맞을때 (수비하던지 회피하던지 하겠지)
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();
    
    void initHeroState(const SGHeroInfo& info);    //게임 한판 시작 전에 꼭 호출을 해서 능력치 초기화를 해야한다.
    
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
    CCActionInterval** act_attack;
    CCActionInterval* act_run, *act_hide, *act_show, *act_keep, *act_wait, *act_defend, *act_die;

//싱글톤패턴
private:
    SGHero(CCLayer* parent);
    static SGHero* sharedSGHero;
    CCLayer* parentLayer;
    
public:
    ~SGHero();
    static SGHero* sharedInstance(CCLayer* parent);
};



#endif /* defined(__SketchGame_TouchInput__SGHero__) */
