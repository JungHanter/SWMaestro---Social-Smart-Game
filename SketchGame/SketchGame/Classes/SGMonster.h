//
//  SGMonster.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#ifndef __SketchGame_TouchInput__SGMonster__
#define __SketchGame_TouchInput__SGMonster__

#include "cocos2d.h"

using namespace cocos2d;

enum SGAttackDirection { ATK_DIR_LEFT, ATK_DIR_RIGHT, ATK_DIR_UP, ATK_DIR_DOWN };

struct SGAttackAction {
    int atkDir;
    CCActionInterval* act_attack;
};


struct SGAttackInfo {
    int atk;
    int atkDir;
};

enum SGMonsterType { MONSTER_TYPE_MUD=0, MONSTER_TYPE_BAT, MONSTER_TYPE_WING, MONSTER_TYPE_BALL, MONSTER_TYPE_NUMBER };

class SGMonster : public CCObject {
public:
    void appear();
    void beginBattle();
    bool endBattle();
    void confirmBattlePos();
    void startBattleMode();
    
    SGAttackInfo attack();   //return now atkDir & atk
    void defend(int damage);
    int die();
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();
    
    int getType();
    bool isWakeupMonster();
    int getInkAmount();
    int getScoreAmount();
    
	void attackComplete(float dt);
    void resetStatus(int hp, int atk, int inkAmount, int scoreAmount);
    void upgradeStatus(float upHpRate, float upAtkRate, float upInkRate, float upScoreRate);
    
public:
    int func_wakeup();
    void func_waiting();
private:
    int selectAttackDirection();
    void func_defend();

    
private:
    int type;
    int maxHP, nowHP;
    int atk;
    int nWakeupFrames;
    int nDieFrames;
    int inkAmount, scoreAmount;
	bool die_flag;
    bool bWakeupMonster;

	void dying(float dt);
    
    CCSprite* monsterSprite;
    CCActionInterval* act_run, *act_wait, *act_defend, *act_die, *act_wakeup;
    
    int numAttacks;
    SGAttackAction* act_attack;  //monster's attack dir array 1~4

private:
    SGMonster(int type, int hp, int atk, int ink, int score, const CCPoint* const movePoints, const int nPoints, CCLayer* const parent);
    CCLayer* const parentLayer;
    
public:
    virtual ~SGMonster();
    static SGMonster* create(int type, int hp, int atk, int ink, int score, const CCPoint* const movePoints, const int nPoints,
                             CCLayer* const parent);
};



#endif /* defined(__SketchGame_TouchInput__SGMonster__) */
