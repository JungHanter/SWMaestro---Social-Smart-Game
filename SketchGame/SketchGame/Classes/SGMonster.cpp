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
	
	die_flag = false;
}


bool SGMonster::endBattle() {
    monsterSprite->setColor(ccc3(255,255,255));
    
	if(nowHP<=0)
	{
		die_flag = true;

		
		//monsterSprite->schedule(schedule_selector(SGMonster::dying), 0.1f);

//		die();
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
	monsterSprite->runAction(
		act_attack[info.atkDir].act_attack);
    
    return info;
}


void SGMonster::defend(int damage) {
	printf("defend monster\n");
	monsterSprite->stopAllActions();
	monsterSprite->runAction(
		act_defend);
	nowHP -= damage;
}

void SGMonster::appear() {
    monsterSprite->runAction(act_run);
}

void SGMonster::die() {
    monsterSprite->stopAllActions();
    monsterSprite->setPosition(ccp(-500,-500));
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
        case MONSTER_TYPE_BAT:
            pSpriteFrameCache->addSpriteFramesWithFile("bat.plist", "bat.png");
            monsterSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bat_wait_1.png"));
            monsterSprite->retain();
            monsterSprite->setPosition(ccp(-500,-500));
            parentLayer->addChild(monsterSprite, ORDER_MONSTER, TAG_TEXTURE);
            
            CCArray* pBatWalkFrames = CCArray::create();
            CCArray* pBatWalkActions = CCArray::create();
            for(int i=1; i<=nPoints; i++) {
                pBatWalkFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_walk_%d.png", i)->getCString()));
            }
            for(int i=0; i<nPoints; i++) {
                pBatWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //mett or Not
                    pBatWalkActions->addObject(CCCallFunc::create(this, callfunc_selector(SGMonster::confirmBattlePos)));
                }
            }
            pBatWalkActions->addObject(CCPlace::create(ccp(-500,-500)));
            act_run = CCSpawn::create(CCAnimate::create(CCAnimation::create(pBatWalkFrames, GAME_FRAME_SPEED)),
                                                     CCSequence::create(pBatWalkActions));
            act_run->retain();
            pBatWalkActions->release();
            pBatWalkFrames->release();
            
            CCArray* pBatDefendFrames = CCArray::create();
            for(int i=1; i<=3; i++) {
                pBatDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_defend_%d.png", i)->getCString()));
            }
            act_defend = CCAnimate::create(CCAnimation::create(pBatDefendFrames,GAME_FRAME_SPEED));
            act_defend->retain();
            pBatDefendFrames->release();
            
            CCArray* pBatDieFrames = CCArray::create();
            for(int i=1; i<=9; i++) {
                pBatDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_die_%d.png", i)->getCString()));
            }
            act_die = CCSequence::create(CCAnimate::create(CCAnimation::create(pBatDieFrames,GAME_FRAME_SPEED)),
                                            CCPlace::create(ccp(-500,-500)));
            act_die->retain();
            pBatDieFrames->release();
            
            CCArray* pBatWaitFrames = CCArray::create();
            for(int i=1; i<=7; i++) {
                pBatWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_wait_%d.png", i)->getCString()));
            }
            for(int i=6; i>=2; i--) {
                pBatWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_wait_%d.png", i)->getCString()));
            }
            act_wait = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pBatWaitFrames,GAME_FRAME_SPEED)));
            act_wait->retain();
            pBatWaitFrames->release();
            
			numAttacks = 2;
            act_attack = new SGAttackAction[numAttacks];

            CCArray* pBatAttackFrames = CCArray::create();
            for(int i=1; i<=10; i++) {
                pBatAttackFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_attack_left_%d.png", i)->getCString()));
            }
			act_attack[0].atkDir = ATK_DIR_LEFT; 
		//	act_attack[0].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pBatAttackFrames,GAME_FRAME_SPEED)),
			//	CCCallFunc::create(callfunc_selector(SGMonster::)));
            act_attack[0].act_attack = CCAnimate::create(CCAnimation::create(pBatAttackFrames,GAME_FRAME_SPEED));
			/*act_attack[0].act_attack = CCSpawn::create(CCAnimate::create(CCAnimation::create(pBatAttackFrames,GAME_FRAME_SPEED)),
             CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
             CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
             CCPlace::create(MONSTER_INIT_POS)),
             CCPlace::create(MONSTER_INIT_POS))));*/
			act_attack[0].act_attack->retain();
            
            CCArray* pBatAttackFrames2 = CCArray::create();
            for(int i=1; i<=10; i++) {
                pBatAttackFrames2->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_attack_right_%d.png", i)->getCString()));
            }
			act_attack[1].atkDir = ATK_DIR_RIGHT;
            act_attack[1].act_attack = CCAnimate::create(CCAnimation::create(pBatAttackFrames2,GAME_FRAME_SPEED));
			act_attack[1].act_attack->retain();
            
            pBatAttackFrames2->release();
            

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