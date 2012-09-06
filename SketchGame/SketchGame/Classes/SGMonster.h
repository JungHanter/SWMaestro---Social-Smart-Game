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

enum SGAttackDirection { ATK_DIR_UP, ATK_DIR_DOWN, ATK_DIR_LEFT, ATK_DIR_RIGHT };

struct SGAttackAction {
    int atkDir;
    CCActionInterval* act_attack;
};


struct SGAttackInfo {
    int atk;
    int atkDir;
};

enum SGMonsterType { MONSTER_TYPE_SPIDER=0, MONSTER_TYPE_NUMBER };

class SGMonster : public CCObject {
public:
    void appear();
    void beginBattle();
    bool endBattle();
    void confirmBattlePos();
    
    SGAttackInfo attack();   //return now atkDir & atk
    void defend(int damage);
    void die();
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();
    
    int getType();
    
    void resetStatus(int hp, int atk);
    void upgradeStatus(float upHpRate, float upAtkRate);
    
private:
    int selectAttackDirection();
    
private:
    int type;
    int maxHP, nowHP;
    int atk;
    
    CCSprite* monsterSprite;
    CCActionInterval* act_run, *act_wait, *act_defend, *act_die;
    
    int numAttacks;
    SGAttackAction* act_attack;  //monster's attack dir array 1~4

private:
    SGMonster(int type, int hp, int atk, const CCPoint* const movePoints, const int nPoints, CCLayer* const parent);
    CCLayer* const parentLayer;
    
public:
    virtual ~SGMonster();
    static SGMonster* create(int type, int hp, int atk, const CCPoint* const movePoints, const int nPoints,
                             CCLayer* const parent);
};



#endif /* defined(__SketchGame_TouchInput__SGMonster__) */
