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
    //monsterSprite->setColor(ccc3(255, 0, 0));
    //monsterSprite->pauseSchedulerAndActions();
    //monsterSprite->stopAllActions();
    //monsterSprite->runAction(act_wait);
	
	die_flag = false;
}


bool SGMonster::endBattle() {
    //monsterSprite->setColor(ccc3(255,255,255));
    
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
        monsterSprite->stopAllActions();
        monsterSprite->runAction(act_wait);
        startBattleMode();
    }
}

void SGMonster::startBattleMode() {
    CCLog("Start Battle");
    ((SketchGameLayer*)parentLayer)->beginBattleMode();
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
void SGMonster::attackComplete(float dt){
	monsterSprite->stopAllActions();
	monsterSprite->runAction(act_run);
}

void SGMonster::defend(int damage) {
	CCLog("defend monster");
	monsterSprite->runAction(CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*4), CCCallFunc::create(this, callfunc_selector(SGMonster::func_defend))));
	nowHP -= damage;
}

void SGMonster::func_defend() {
    monsterSprite->stopAllActions();
    monsterSprite->runAction(act_defend);
}

void SGMonster::appear() {
    monsterSprite->runAction(act_run);
}

int SGMonster::die() {
    monsterSprite->stopAllActions();
    monsterSprite->runAction(act_die);
    
    return nDieFrames;
}

int SGMonster::func_wakeup() {
    monsterSprite->stopAllActions();
    monsterSprite->runAction(act_wakeup);
    
    return nWakeupFrames;
}

int SGMonster::selectAttackDirection() {
    //test
	return rand()%this->numAttacks;
}

void SGMonster::func_waiting() {
    monsterSprite->stopAllActions();
    monsterSprite->runAction(act_wait);
}

bool SGMonster::isWakeupMonster() {
    return bWakeupMonster;
}

int SGMonster::getInkAmount() {
    return inkAmount;
}

int SGMonster::getScoreAmount() {
    return scoreAmount;
}

void SGMonster::resetStatus(int hp, int atk, int inkAmount, int scoreAmount) {
    die_flag = false;
    this->maxHP = hp;
    this->nowHP = this->maxHP;
    this->atk = atk;
    this->inkAmount = inkAmount;
    this->scoreAmount = scoreAmount;
}


void SGMonster::upgradeStatus(float upHpRate, float upAtkRate, float upInkRate, float upScoreRate) {
    this->maxHP = (int)(((float)(this->maxHP))*upHpRate);
    this->nowHP = this->maxHP;
    this->atk = (int)(((float)(this->atk))*upAtkRate);
    this->inkAmount = (int)(((float)(this->inkAmount))*upInkRate);
    this->scoreAmount = (int)(((float)(this->scoreAmount))*upScoreRate);

}

void SGMonster::pauseSchedulerAndActions() {
    monsterSprite->pauseSchedulerAndActions();
}

void SGMonster::resumeSchedulerAndActions() {
    monsterSprite->resumeSchedulerAndActions();
}

SGMonster::SGMonster(int type, int hp, int atk, int inkAmount, int score, const CCPoint* const movePoints,
                     const int nPoints, CCLayer* const parent) : parentLayer(parent) {
    this->maxHP = hp;
    this->nowHP = this->maxHP;
    this->atk = atk;
    
    this->bWakeupMonster = false;
    this->nWakeupFrames = -1;
    this->act_wakeup = NULL;
    
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    switch (type) {
        case MONSTER_TYPE_MUD: {
            monsterSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("mud_wait_1.png"));
            monsterSprite->retain();
            monsterSprite->setPosition(ccp(-500,-500));
            parentLayer->addChild(monsterSprite, ORDER_MONSTER, TAG_TEXTURE);
            
            CCArray* pWalkFrames = CCArray::create();
            CCArray* pWalkActions = CCArray::create();
            for(int i=1; i<=nPoints; i++) {
                pWalkFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_walk_%d.png", i)->getCString()));
            }
            for(int i=0; i<nPoints; i++) {
                pWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //mett or Not
                    pWalkActions->addObject(CCCallFunc::create(this, callfunc_selector(SGMonster::confirmBattlePos)));
                }
            }
            pWalkActions->addObject(CCPlace::create(ccp(-500,-500)));
            act_run = CCSpawn::create(CCAnimate::create(CCAnimation::create(pWalkFrames, GAME_FRAME_SPEED)),
                                      CCSequence::create(pWalkActions));
            act_run->retain();
            pWalkActions->release();
            pWalkFrames->release();
            
            CCArray* pWaitFrames = CCArray::create();
            for(int i=1; i<=3; i++) {
                pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_wait_%d.png", i)->getCString()));
            }
            pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName("mud_wait_2.png"));
            act_wait = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pWaitFrames,GAME_FRAME_SPEED)));
            act_wait->retain();
            pWaitFrames->release();
            
            bWakeupMonster = true;
            nWakeupFrames = 6;
            CCArray* pWakeupFrames = CCArray::create();
            for(int i=1; i<=6; i++) {
                pWakeupFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_wakeup_%d.png", i)->getCString()));
            }
            //act_wakeup = CCAnimate::create(CCAnimation::create(pWakeupFrames,GAME_FRAME_SPEED));
            act_wakeup = CCSequence::create(CCAnimate::create(CCAnimation::create(pWakeupFrames,GAME_FRAME_SPEED)),
                                            CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
            act_wakeup->retain();
            pWakeupFrames->release();
            
            CCArray* pDefendFrames = CCArray::create();
            for(int i=1; i<=4; i++) {
                pDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_defend_%d.png", i)->getCString()));
            }
            act_defend = CCSequence::create(CCAnimate::create(CCAnimation::create(pDefendFrames,GAME_FRAME_SPEED)),
                                            CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
            act_defend->retain();
            pDefendFrames->release();
            
            CCArray* pDieFrames = CCArray::create();
            for(int i=1; i<=9; i++) {
                pDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_die_%d.png", i)->getCString()));
            }
            act_die = CCSequence::create(CCAnimate::create(CCAnimation::create(pDieFrames,GAME_FRAME_SPEED)),
                                         CCPlace::create(ccp(-500,-500)));
            act_die->retain();
            pDieFrames->release();
            nDieFrames=9;
            
			numAttacks = 1;
            act_attack = new SGAttackAction[numAttacks];
          
            CCArray* pAttackRightFrames = CCArray::create();
            for(int i=1; i<=7; i++) {
                pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_attack_right_%d.png", i)->getCString()));
            }
            for(int i=4; i>=1; i--) {
                pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("mud_attack_right_%d.png", i)->getCString()));
            }
			act_attack[0].atkDir = ATK_DIR_RIGHT;
            act_attack[0].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackRightFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[0].act_attack->retain();
            pAttackRightFrames->release();
            
            break;
        }
            
        case MONSTER_TYPE_BAT: {
            monsterSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bat_wait_1.png"));
            monsterSprite->retain();
            monsterSprite->setPosition(ccp(-500,-500));
            parentLayer->addChild(monsterSprite, ORDER_MONSTER, TAG_TEXTURE);
            
            CCArray* pWalkFrames = CCArray::create();
            CCArray* pWalkActions = CCArray::create();
            for(int i=1; i<=nPoints; i++) {
                pWalkFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_walk_%d.png", i)->getCString()));
            }
            for(int i=0; i<nPoints; i++) {
                pWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //mett or Not
                    pWalkActions->addObject(CCCallFunc::create(this, callfunc_selector(SGMonster::confirmBattlePos)));
                }
            }
            pWalkActions->addObject(CCPlace::create(ccp(-500,-500)));
            act_run = CCSpawn::create(CCAnimate::create(CCAnimation::create(pWalkFrames, GAME_FRAME_SPEED)),
                                      CCSequence::create(pWalkActions));
            act_run->retain();
            pWalkActions->release();
            pWalkFrames->release();
            
            CCArray* pWaitFrames = CCArray::create();
            for(int i=1; i<=7; i++) {
                pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_wait_%d.png", i)->getCString()));
            }
            for(int i=6; i>=2; i--) {
                pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_wait_%d.png", i)->getCString()));
            }
            act_wait = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pWaitFrames,GAME_FRAME_SPEED)));
            act_wait->retain();
            pWaitFrames->release();
            
            CCArray* pDefendFrames = CCArray::create();
            for(int i=1; i<=3; i++) {
                pDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_defend_%d.png", i)->getCString()));
            }
            act_defend = CCSequence::create(CCAnimate::create(CCAnimation::create(pDefendFrames,GAME_FRAME_SPEED)),
                                            CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
            act_defend->retain();
            pDefendFrames->release();
            
            CCArray* pDieFrames = CCArray::create();
            for(int i=1; i<=9; i++) {
                pDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_die_%d.png", i)->getCString()));
            }
            act_die = CCSequence::create(CCAnimate::create(CCAnimation::create(pDieFrames,GAME_FRAME_SPEED)),
                                         CCPlace::create(ccp(-500,-500)));
            act_die->retain();
            pDieFrames->release();
            nDieFrames=9;
            
			numAttacks = 2;
            act_attack = new SGAttackAction[numAttacks];
            
            CCArray* pAttackLeftFrames = CCArray::create();
            for(int i=1; i<=11; i++) {
                pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_attack_left_%d.png", i)->getCString()));
            }
			pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_wait_2.png")->getCString()));
			act_attack[0].atkDir = ATK_DIR_LEFT;
			act_attack[0].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackLeftFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[0].act_attack->retain();
            pAttackLeftFrames->release();
            
            CCArray* pAttackRightFrames = CCArray::create();
            for(int i=1; i<=11; i++) {
                pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_attack_right_%d.png", i)->getCString()));
            }
			pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bat_wait_2.png")->getCString()));
			act_attack[1].atkDir = ATK_DIR_RIGHT;
            act_attack[1].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackRightFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[1].act_attack->retain();
            pAttackRightFrames->release();
            
            break;
        }
            
        case MONSTER_TYPE_WING: {
            monsterSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("wing_wait_1.png"));
            monsterSprite->retain();
            monsterSprite->setPosition(ccp(-500,-500));
            parentLayer->addChild(monsterSprite, ORDER_MONSTER, TAG_TEXTURE);
            
            CCArray* pWalkFrames = CCArray::create();
            CCArray* pWalkActions = CCArray::create();
            for(int i=1; i<=nPoints; i++) {
                pWalkFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_walk_%d.png", i)->getCString()));
            }
            for(int i=0; i<nPoints; i++) {
                pWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //mett or Not
                    pWalkActions->addObject(CCCallFunc::create(this, callfunc_selector(SGMonster::confirmBattlePos)));
                }
            }
            pWalkActions->addObject(CCPlace::create(ccp(-500,-500)));
            act_run = CCSpawn::create(CCAnimate::create(CCAnimation::create(pWalkFrames, GAME_FRAME_SPEED)),
                                      CCSequence::create(pWalkActions));
            act_run->retain();
            pWalkActions->release();
            pWalkFrames->release();
            
            CCArray* pWaitFrames = CCArray::create();
            for(int i=1; i<=5; i++) {
                pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_wait_%d.png", i)->getCString()));
            }
            for(int i=4; i>=2; i--) {
                pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_wait_%d.png", i)->getCString()));
            }
            act_wait = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pWaitFrames,GAME_FRAME_SPEED)));
            act_wait->retain();
            pWaitFrames->release();
            
            CCArray* pDefendFrames = CCArray::create();
            for(int i=1; i<=3; i++) {
                pDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_defend_%d.png", i)->getCString()));
            }
            act_defend = CCSequence::create(CCAnimate::create(CCAnimation::create(pDefendFrames,GAME_FRAME_SPEED)),
                                            CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
            act_defend->retain();
            pDefendFrames->release();
            
            CCArray* pDieFrames = CCArray::create();
            for(int i=1; i<=8; i++) {
                pDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_die_%d.png", i)->getCString()));
            }
            act_die = CCSequence::create(CCAnimate::create(CCAnimation::create(pDieFrames,GAME_FRAME_SPEED)),
                                         CCPlace::create(ccp(-500,-500)));
            act_die->retain();
            pDieFrames->release();
            nDieFrames=8;
            
			numAttacks = 3;
            act_attack = new SGAttackAction[numAttacks];
            
            CCArray* pAttackLeftFrames = CCArray::create();
            pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName("wing_attack_1.png"));
            for(int i=2; i<=5; i++) {
                pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_attack_left_%d.png", i)->getCString()));
            }
			act_attack[0].atkDir = ATK_DIR_LEFT;
			act_attack[0].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackLeftFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[0].act_attack->retain();
            pAttackLeftFrames->release();
            
            CCArray* pAttackRightFrames = CCArray::create();
            pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName("wing_attack_1.png"));
            for(int i=2; i<=5; i++) {
                pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_attack_right_%d.png", i)->getCString()));
            }
			act_attack[1].atkDir = ATK_DIR_RIGHT;
            act_attack[1].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackRightFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[1].act_attack->retain();
            pAttackRightFrames->release();
            
            CCArray* pAttackUpFrames = CCArray::create();
            pAttackUpFrames->addObject(pSpriteFrameCache->spriteFrameByName("wing_attack_up_.png"));
            for(int i=3; i<=7; i++) {
                pAttackUpFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("wing_attack_up_%d.png", i)->getCString()));
            }
            pAttackUpFrames->addObject(pSpriteFrameCache->spriteFrameByName("wing_attack_up_4.png"));
            pAttackUpFrames->addObject(pSpriteFrameCache->spriteFrameByName("wing_attack_up_3.png"));
			act_attack[2].atkDir = ATK_DIR_UP;
            act_attack[2].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackUpFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[2].act_attack->retain();
            pAttackUpFrames->release();
            
            break;
        }
            
        case MONSTER_TYPE_BALL: {
            monsterSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("ball_wait_1.png"));
            monsterSprite->retain();
            monsterSprite->setPosition(ccp(-500,-500));
            parentLayer->addChild(monsterSprite, ORDER_MONSTER, TAG_TEXTURE);
            
            CCArray* pWalkFrames = CCArray::create();
            CCArray* pWalkActions = CCArray::create();
            for(int i=1; i<=nPoints; i++) {
                pWalkFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_walk_%d.png", i)->getCString()));
            }
            for(int i=0; i<nPoints; i++) {
                pWalkActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(movePoints[i])));
                if(i==12) {
                    //mett or Not
                    pWalkActions->addObject(CCCallFunc::create(this, callfunc_selector(SGMonster::confirmBattlePos)));
                }
            }
            pWalkActions->addObject(CCPlace::create(ccp(-500,-500)));
            act_run = CCSpawn::create(CCAnimate::create(CCAnimation::create(pWalkFrames, GAME_FRAME_SPEED)),
                                      CCSequence::create(pWalkActions));
            act_run->retain();
            pWalkActions->release();
            pWalkFrames->release();
            
            CCArray* pWaitFrames = CCArray::create();
            for(int i=1; i<=3; i++) {
                pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_wait_%d.png", i)->getCString()));
            }
            pWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName("ball_wait_2.png"));
            act_wait = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pWaitFrames, GAME_FRAME_SPEED)));
            act_wait->retain();
            pWaitFrames->release();
            
            CCArray* pDefendFrames = CCArray::create();
            for(int i=1; i<=2; i++) {
                pDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_defend_%d.png", i)->getCString()));
            }
            act_defend = CCSequence::create(CCAnimate::create(CCAnimation::create(pDefendFrames,GAME_FRAME_SPEED)),
                                            CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
            act_defend->retain();
            pDefendFrames->release();
            
            CCArray* pDieFrames = CCArray::create();
            for(int i=1; i<=8; i++) {
                pDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_die_%d.png", i)->getCString()));
            }
            act_die = CCSequence::create(CCAnimate::create(CCAnimation::create(pDieFrames,GAME_FRAME_SPEED)),
                                         CCPlace::create(ccp(-500,-500)));
            act_die->retain();
            pDieFrames->release();
            nDieFrames=8;
            
			numAttacks = 4;
            act_attack = new SGAttackAction[numAttacks];
            
            CCArray* pAttackLeftFrames = CCArray::create();
            for(int i=1; i<=7; i++) {
                pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_attack_left_%d.png", i)->getCString()));
            }
			pAttackLeftFrames->addObject(pSpriteFrameCache->spriteFrameByName("ball_attack_right_1.png"));
			act_attack[0].atkDir = ATK_DIR_LEFT;
			act_attack[0].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackLeftFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[0].act_attack->retain();
            pAttackLeftFrames->release();
            
            CCArray* pAttackRightFrames = CCArray::create();
            for(int i=1; i<=7; i++) {
                pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_attack_right_%d.png", i)->getCString()));
            }
			pAttackRightFrames->addObject(pSpriteFrameCache->spriteFrameByName("ball_attack_left_1.png"));
			act_attack[1].atkDir = ATK_DIR_RIGHT;
            act_attack[1].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackRightFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[1].act_attack->retain();
            pAttackRightFrames->release();
            
            CCArray* pAttackUpFrames = CCArray::create();
            for(int i=1; i<=5; i++) {
                pAttackUpFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_attack_up_%d.png", i)->getCString()));
            }
            pAttackUpFrames->addObject(pSpriteFrameCache->spriteFrameByName("ball_attack_up_2.png"));
			act_attack[2].atkDir = ATK_DIR_UP;
            act_attack[2].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackUpFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[2].act_attack->retain();
            pAttackUpFrames->release();
            
            CCArray* pAttackDownFrames = CCArray::create();
            for(int i=1; i<=5; i++) {
                pAttackDownFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ball_attack_down_%d.png", i)->getCString()));
            }
			act_attack[3].atkDir = ATK_DIR_DOWN;
            act_attack[3].act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pAttackDownFrames,GAME_FRAME_SPEED)),
                                                          CCCallFunc::create(this, callfunc_selector(SGMonster::func_waiting)));
			act_attack[3].act_attack->retain();
            pAttackDownFrames->release();
            break;
        }
    }
}

SGMonster::~SGMonster() {

}

int SGMonster::getType() {
    return type;
}

SGMonster* SGMonster::create(int type, int hp, int atk, int inkAmount, int score, const CCPoint* const movePoints,
                              const int nPoints, CCLayer* const parent) {
    SGMonster* monster = new SGMonster(type, hp, atk, inkAmount, score, movePoints, nPoints, parent);
    
    monster->autorelease();
    
    return monster;
}