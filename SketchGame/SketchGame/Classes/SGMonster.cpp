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
    monsterSprite->setColor(ccc3(255, 0, 0));
    
    monsterSprite->pauseSchedulerAndActions();
}

bool SGMonster::endBattle() {
    monsterSprite->setColor(ccc3(255,255,255));
    
	if(nowHP<=0)
	{
		die();
        return true;
	}
	else
		return false;
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
    
    monsterSprite->stopAllActions();
	monsterSprite->runAction(act_attack[0].act_attack);
    
    return info;
}


void SGMonster::defend(int damage) {

	nowHP -= damage;
}

void SGMonster::appear() {
    monsterSprite->runAction(act_run);
}

void SGMonster::die() {
    monsterSprite->stopAllActions();
    monsterSprite->setPosition(ccp(-100,-100));
}

int SGMonster::selectAttackDirection() {
    //test
    return ATK_DIR_LEFT;
}


void SGMonster::resetStatus(int hp, int atk) {
    this->maxHP = hp;
    this->nowHP = this->maxHP;
    this->atk = atk;
}


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
            for(int i=0; i<nPoints; i++) {
                pSpiderWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //mett or Not
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

			CCArray* pHeroActAttackFrames = CCArray::create();
			for(int i=1; i<=4; i++) {
				pHeroActAttackFrames->addObject(pSpriteFrameCache->spriteFrameByName(
					CCString::createWithFormat("spider_attack_%d.png",i)->getCString()));
			}
			
			act_attack[0].act_attack = CCSpawn::create(CCAnimate::create(CCAnimation::create(pSpiderAttackFrames,GAME_FRAME_SPEED)),
				CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
				CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(MONSTER_INIT_POS)),
				CCPlace::create(MONSTER_INIT_POS))));
			act_attack[0].act_attack->retain();
            
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