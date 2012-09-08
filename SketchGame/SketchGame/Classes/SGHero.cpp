//
//  SGHero.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#include "SGHero.h"
#include "defines.h"

SGHero* SGHero::sharedSGHero = NULL;

void SGHero::beginBattle() {

}

void SGHero::endBattle() {
    
}

void SGHero::hideHeroObject() {
    
}

void SGHero::confirmBattleMode() {
    
}

int SGHero::attack() {
	heroSprite->stopAllActions();

	heroSprite->runAction(act_attack);
    return atk;
}

void SGHero::setDefendAction(int act,int dodgeDirection){
	//heroSprite->stopAllActions();
	switch (act) {
        case DEF_STATE_DEFEND:
			defendState = DEF_STATE_DEFEND;
	//		heroSprite->runAction(act_wait);
            break;
        case DEF_STATE_GUARD:
			defendState = DEF_STATE_GUARD;
		//	heroSprite->runAction(act_defend);
            break;
        case DEF_STATE_DODGE:
			defendState = DEF_STATE_DODGE;
			this->dodgeDirection = dodgeDirection;
	//		heroSprite->runAction(act_hide);
            break;
    }
}
void SGHero::defend(int damage,int mob_direction) {
    //맞는 액션
	//heroSprite->stopAllActions();

    //체력감소
    switch (defendState) {
		case DEF_STATE_NONE:
        case DEF_STATE_DEFEND:
			heroSprite->runAction(act_defend);
            nowHP-=damage;
            break;
        case DEF_STATE_GUARD:
            nowHP-=(damage/2);
			heroSprite->runAction(act_block);
            break;
        case DEF_STATE_DODGE:
			
			if(mob_direction == dodgeDirection)
				nowHP-=damage;
			else{
				nowHP-=(damage/10);
				heroSprite->runAction(act_hide);
			}
            break;
    }
}

void SGHero::pauseSchedulerAndActions() {
    heroSprite->pauseSchedulerAndActions();
}

void SGHero::resumeSchedulerAndActions() {
    heroSprite->resumeSchedulerAndActions();
}

void SGHero::func_startHide() {
    heroSprite->stopAllActions();
    
    heroSprite->runAction(act_hide);
}

void SGHero::func_MoveHide() {
    heroSprite->stopAllActions();
    heroSprite->runAction(act_keep);
}

void SGHero::func_MoveShow() {
    heroSprite->stopAllActions();
    
    heroSprite->runAction(act_show);
    //heroSprite->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_INIT_POS)));
}

void SGHero::func_startRun() {
    heroSprite->stopAllActions();
    heroSprite->setPosition(HERO_INIT_POS);
    heroSprite->runAction(act_run);
}

void SGHero::initHeroState(const SGHeroInfo &info) {
    this->maxHP = 20 + (info.Con);
    this->nowHP = this->maxHP;
    this->atk = 5 + (info.Str/4);
}

SGHero::SGHero(CCLayer* parent) : parentLayer(parent) {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pSpriteFrameCache->addSpriteFramesWithFile("hero.plist", "hero.png");
    
    heroSprite = CCSprite::create(pSpriteFrameCache->spriteFrameByName("hero_keep.png"));
    heroSprite->retain();
    heroSprite->setPosition(HERO_INIT_POS);
    parentLayer->addChild(heroSprite, ORDER_HERO, TAG_TEXTURE);
    
    CCArray* pHeroActRunFrames = CCArray::create();
    for(int i=1; i<=7; i++) {
        pHeroActRunFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_walk_%d.png",i)->getCString()));
    }
    act_run = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pHeroActRunFrames, GAME_FRAME_SPEED)));
    act_run->retain();
    heroSprite->runAction(act_run);
    pHeroActRunFrames->release();
    
    CCArray* pHeroActKeepStoneFrames = CCArray::create();
    pHeroActKeepStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_keep.png"));
    act_keep = CCAnimate::create(CCAnimation::create(pHeroActKeepStoneFrames));
    act_keep->retain();
    pHeroActKeepStoneFrames->release();
    
    CCArray* pHeroActWaitFrames = CCArray::create();
    for(int i=1; i<=4; i++) {
        pHeroActWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_wait_%d.png",i)->getCString()));
    }
    pHeroActWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_wait_3.png"));
    pHeroActWaitFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_wait_2.png"));
    act_wait = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pHeroActWaitFrames, GAME_FRAME_SPEED)));
    act_wait->retain();
    pHeroActWaitFrames->release();
    
    //hero->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_HIDE_DEST_POS)));
    //hero->ru`nAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_INIT_POS)));
    CCArray* pHeroActHideStoneFrames = CCArray::create();
    pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    for(int i=1; i<=9; i++) {
        pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_hide_%d.png",10-i)->getCString()));
    }
    act_hide = CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActHideStoneFrames, GAME_FRAME_SPEED)),
                               CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                  CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                     CCPlace::create(HERO_HIDE_DEST_POS))));
    act_hide->retain();
    pHeroActHideStoneFrames->release();
    
    CCArray* pHeroActShowStoneFrames = CCArray::create();
    for(int i=1; i<=9; i++) {
        pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_hide_%d.png",i)->getCString()));
    }
    pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    act_show = CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActShowStoneFrames, GAME_FRAME_SPEED)),
                               CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                  CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                     CCPlace::create(HERO_INIT_POS))));
    act_show->retain();
    pHeroActShowStoneFrames->release();
  

    CCArray* pHeroActBlockFrames = CCArray::create();
	for(int i=1; i<=9; i++) {
		pHeroActBlockFrames->addObject(pSpriteFrameCache->spriteFrameByName(
			CCString::createWithFormat("hero_block_%d.png",i)->getCString()));
	}
    act_block = CCAnimate::create(CCAnimation::create(pHeroActBlockFrames, GAME_FRAME_SPEED));
	/*act_defend = CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActBlockFrames, GAME_FRAME_SPEED)),
                                 CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                    CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_INIT_POS)),
                                                                       CCPlace::create(HERO_INIT_POS))));*/
	act_block->retain();
    pHeroActBlockFrames->release();

	CCArray* pHeroActDefendFrames = CCArray::create();
	for(int i=1; i<=3; i++) {
		pHeroActDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(
			CCString::createWithFormat("hero_defend_%d.png",i)->getCString()));
	}
    act_defend = CCAnimate::create(CCAnimation::create(pHeroActDefendFrames, GAME_FRAME_SPEED));
	act_defend->retain();
    pHeroActDefendFrames->release();


	CCArray* pHeroActAttackFrames = CCArray::create();
	for(int i=1; i<=9; i++) {
		pHeroActAttackFrames->addObject(pSpriteFrameCache->spriteFrameByName(
			CCString::createWithFormat("hero_attack_%d.png",i)->getCString()));
	}
    act_attack = CCAnimate::create(CCAnimation::create(pHeroActAttackFrames, GAME_FRAME_SPEED/1.5f));
/*	act_attack = CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActAttackFrames, GAME_FRAME_SPEED)),
                                 CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                    CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_INIT_POS)),
                                                                       CCPlace::create(HERO_INIT_POS))));*/
	act_attack->retain();
    pHeroActAttackFrames->release();

	defendState = DEF_STATE_NONE;
	dodgeDirection = -1;
    
    this->autorelease();
    this->retain();
}

SGHero::~SGHero() {
    heroSprite->release();
    act_run->release();

    act_keep->release();
    act_hide->release();
    act_show->release();
    
    sharedSGHero = NULL;
    this->release();
}

SGHero* SGHero::sharedInstance(CCLayer* parent) {
    if(sharedSGHero == NULL) {
        sharedSGHero = new SGHero(parent);
    }
    
    return sharedSGHero;
}