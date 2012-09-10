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

#include "SGMonster.h"

struct SGHeroInfo {
    int Str;
    int Con;
    int Dex;
    int Luck;
    unsigned long long ink;
    unsigned long long score[10];
};

enum HeroDodgeDirection {
			DODGE_LEFT, DODGE_RIGHT, DODGE_UP, DODGE_DOWN
		};
enum HeroDefendState {
			DEF_STATE_DEFEND=0, DEF_STATE_GUARD, DEF_STATE_DODGE, DEF_STATE_DIE
		};

class SGHero : public CCObject {
public:
    void hideHeroObject();
    void confirmBattleMode();
    void beginBattle();
    void endBattle();
    
    int attack();  //Attack, return atk
	void setDefendAction(int ,int =DODGE_LEFT);
    bool defend(int damage, int def_state);
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();

    void initHeroState(const SGHeroInfo& info);
    
    int getRemainHpDivision();
    

public:
    void func_startHide();
    void func_MoveHide();
    void func_MoveShow();
    void func_startRun();

	int defendState;
	int dodgeC;

private:
    int actState;
	int dodgeDirection;
    int maxHP, nowHP;
    int atk;
    
    CCSprite* heroSprite;
    CCActionInterval* act_attack;
    CCActionInterval* act_run, *act_hide, *act_show, *act_keep, *act_wait,
		*act_dodge,
		*act_block,*act_defend, *act_die;

public:
    void func_wating();
    void func_stop();
    void func_guard();
    void func_dodge();
private:
    void func_defending(CCObject* act);
    void func_die();
    
private:
    SGHero(CCLayer* parent);
    CCLayer* parentLayer;
    
public:
    virtual ~SGHero();
    static SGHero* create(CCLayer* parent);
};



#endif /* defined(__SketchGame_TouchInput__SGHero__) */
