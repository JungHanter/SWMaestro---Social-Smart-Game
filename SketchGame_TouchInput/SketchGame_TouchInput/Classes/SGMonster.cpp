//
//  SGMonster.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#include "SGMonster.h"
#include "defines.h"
#include "SketchGameScene.h"

void SGMonster::beginBattle() {
    //색깔은 일단 디버깅용으로 입힘
    monsterSprite->setColor(ccc3(255, 0, 0));
    
    //실제 전투는 구현이 안되있으므로 잠시 멈춤
    monsterSprite->pauseSchedulerAndActions();
}

//죽었을때 true 리턴
bool SGMonster::endBattle() {
    monsterSprite->setColor(ccc3(255,255,255));
    
    //실제 전투는 구현이 안되있으므로 잠시 멈춘걸 다시 재생
    monsterSprite->resumeSchedulerAndActions();
    
    //죽었다고 가정
    return true;
}

void SGMonster::confirmBattlePos() {
    if(((SketchGameLayer*)parentLayer)->getGameState() == GAMESTATE_RUNNING) {
        ((SketchGameLayer*)parentLayer)->beginBattleMode();
    }
}

SGAttackInfo SGMonster::attack() {
    SGAttackInfo info;
    info.atk = this->atk;
    info.atkDir = selectAttackDirection();
    
    //공격 수행 스케줄러 작성하면 될듯
    
    
    return info;
}

void SGMonster::defend() {
    
}

void SGMonster::appear() {
    monsterSprite->runAction(act_run);
}

void SGMonster::die() {
    
}

int SGMonster::selectAttackDirection() {
    //test
    return ATK_DIR_LEFT;
}

//몬스터 재활용하기위해 상태 리셋
void SGMonster::resetStatus(int hp, int atk) {
    this->maxHP = hp;
    this->nowHP = this->maxHP;
    this->atk = atk;
}

//몬스터 재활용/능력치 업그레이드
void SGMonster::upgradeStatus(float upHpRate, float upAtkRate) {
    this->maxHP = (int)(((float)(this->maxHP))*upHpRate);
    this->nowHP = this->maxHP;
    this->atk = (int)(((float)(this->atk))*upAtkRate);
}

void SGMonster::pauseSchedulerAndActions() {
    monsterSprite->pauseSchedulerAndActions();
}

void SGMonster::resumeSchedulerAndActions() {
    monsterSprite->resumeSchedulerAndActions();
}

SGMonster::SGMonster(int type, int hp, int atk, const CCPoint* const movePoints,
                     const int nPoints, CCLayer* const parent) : parentLayer(parent) {
    this->maxHP = hp;
    this->nowHP = this->maxHP;
    this->atk = atk;
    
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    switch (type) {
        case MONSTER_TYPE_SPIDER:
            pSpriteFrameCache->addSpriteFramesWithFile("spider.plist", "spider.png");
            monsterSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("spider_walk_1.png"));
            monsterSprite->retain();
            monsterSprite->setPosition(ccp(-500,-500));
            parentLayer->addChild(monsterSprite, ORDER_MONSTER, TAG_TEXTURE);
            
            CCArray* pSpiderWalkFrames = CCArray::create();
            CCArray* pSpiderWalkActions = CCArray::create();
            for(int i=1; i<=nPoints; i++) {
                pSpiderWalkFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("spider_walk_%d.png", i)->getCString()));
            }
            //지수함수그래프 N등분일때 4배수 계수적용
            for(int i=0; i<nPoints; i++) {
                pSpiderWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //12프레임에서 배틀 or NOT
                    pSpiderWalkActions->addObject(CCCallFunc::create(this, callfunc_selector(SGMonster::confirmBattlePos)));
                }
            }
            pSpiderWalkActions->addObject(CCPlace::create(ccp(-500,-500)));
            act_run = CCSpawn::create(CCAnimate::create(CCAnimation::create(pSpiderWalkFrames, GAME_FRAME_SPEED)),
                                                     CCSequence::create(pSpiderWalkActions));
            act_run->retain();
            pSpiderWalkActions->release();
            pSpiderWalkFrames->release();
            
            CCArray* pSpiderAttackFrames = CCArray::create();
            for(int i=1; i<=4; i++) {
                pSpiderAttackFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("spider_attack_%d.png", i)->getCString()));
            }
            
            numAttacks = 1;
            act_attack = new SGAttackAction[numAttacks];
            act_attack[0].act_attack = CCAnimate::create(CCAnimation::create(pSpiderWalkFrames));
            act_attack[0].act_attack->retain();
            act_attack[0].atkDir = ATK_DIR_LEFT;
            pSpiderAttackFrames->retain();
            break;
    }
}

SGMonster::~SGMonster() {
}

int SGMonster::getType() {
    return type;
}

SGMonster* SGMonster::create(int type, int hp, int atk, const CCPoint* const movePoints,
                              const int nPoints, CCLayer* const parent) {
    SGMonster* monster = new SGMonster(type, hp, atk, movePoints, nPoints, parent);
    
    monster->autorelease();
    
    return monster;
}