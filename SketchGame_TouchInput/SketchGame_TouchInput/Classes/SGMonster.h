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

struct SGAttackInfo {
    int atkDir;
    CCActionInterval* act_attack;
};


enum SGMonsterType { MONSTER_TYPE_SPIDER };

class SGMonster : public CCObject {
public:
    void appear();  //몬스터 출혈 시작
    void beginBattle(); //몬스터 배틀모드
    
    void pauseSchedulerAndActions();
    void resumeSchedulerAndActions();
    
private:
    int type;
    int maxHP, nowHP;
    int atk;
    
    CCSprite* monsterSprite;
    CCActionInterval* act_run, *act_wait, *act_defend, *act_die;
    
    char numAttacks;
    SGAttackInfo* act_attack;  //몬스터 공격 배열 (1~4);

private:
    SGMonster();
    
public:
    ~SGMonster();
    static SGMonster create(int type, int hp, int atk, int def);
};



#endif /* defined(__SketchGame_TouchInput__SGMonster__) */
