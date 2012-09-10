//
//  SGHero.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#include "SGHero.h"
#include "defines.h"
int SGHero::attack() {
	heroSprite->stopAllActions();
    
	heroSprite->runAction(act_attack);
    return atk;
}

//if hero die, return false;
bool SGHero::defend(int damage, int def_state) {
	//heroSprite->stopAllActions();
    
    CCActionInterval* nowAction;
    
    switch (def_state) {
        case DEF_STATE_DEFEND:
            nowAction = act_defend;
            nowHP-=damage;
            break;
        case DEF_STATE_GUARD:
            nowHP-=(damage/2);
            nowAction = act_block;
            break;
        case DEF_STATE_DODGE:
            nowAction = act_dodge;
            nowHP-=(damage/10);
            break;
    }
    
    if(nowHP <= 0 ) {
        heroSprite->runAction(CCCallFunc::create(this, callfunc_selector(SGHero::func_die)));
        
        return false;
    } else {
        heroSprite->runAction(CCCallFuncO::create(this, callfuncO_selector(SGHero::func_defending), nowAction));
        return true;;
    }
}


void SGHero::pauseSchedulerAndActions() {
    heroSprite->pauseSchedulerAndActions();
}

void SGHero::resumeSchedulerAndActions() {
    heroSprite->resumeSchedulerAndActions();
}

void SGHero::func_defending(CCObject* _act) {
    CCActionInterval* act = (CCActionInterval*)_act;
    heroSprite->stopAllActions();
    heroSprite->runAction(act);
}

void SGHero::func_startHide() {
    heroSprite->stopAllActions();
    
    heroSprite->runAction(act_hide);
}

void SGHero::func_MoveHide() {
    heroSprite->stopAllActions();
    heroSprite->runAction(act_wait);
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

void SGHero::func_wating() {
    heroSprite->stopAllActions();
    heroSprite->runAction(act_wait);
}

void SGHero::func_die() {
    heroSprite->stopAllActions();
    heroSprite->runAction(act_die);
}

void SGHero::func_stop() {
    //heroSprite->pauseSchedulerAndActions();
    //heroSprite->unscheduleAllSelectors();
    heroSprite->stopAllActions();
}

void SGHero::func_guard() {
    heroSprite->stopAllActions();
    heroSprite->runAction(act_block);
}

void SGHero::func_dodge() {
    heroSprite->stopAllActions();
    heroSprite->runAction(act_dodge);
}

int SGHero::getRemainHpDivision() {
    int div = (int)(((float)nowHP/(float)maxHP)*10.f);
    return div;
}

void SGHero::initHeroState(const SGHeroInfo &info) {
    this->maxHP = 20 + (info.Con);
    this->nowHP = this->maxHP;
    this->atk = 5 + (info.Str/4);
}

SGHero::SGHero(CCLayer* parent) : parentLayer(parent) {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
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
    act_hide = CCSequence::create(CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActHideStoneFrames, GAME_FRAME_SPEED)),
                                                  CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                                     CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                                        CCPlace::create(HERO_HIDE_DEST_POS)))),
                                  CCCallFunc::create(this, callfunc_selector(SGHero::func_wating)));
    act_hide->retain();
    pHeroActHideStoneFrames->release();
    
    CCArray* pHeroActShowStoneFrames = CCArray::create();
    for(int i=1; i<=9; i++) {
        pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                                CCString::createWithFormat("hero_hide_%d.png",i)->getCString()));
    }
    pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    act_show = CCSequence::create(CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActShowStoneFrames, GAME_FRAME_SPEED)),
                                                  CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                                     CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                                        CCPlace::create(HERO_INIT_POS)))),
                                  CCCallFunc::create(this, callfunc_selector(SGHero::func_wating)));
    act_show->retain();
    pHeroActShowStoneFrames->release();
    
    
    ///////////////////////////////////////////////////////DODGE///////////////////////////////////////////////////////////////////////////////////////////////////
    CCArray* pHeroDodgeFrames = CCArray::create();
    CCArray* pHeroDodgeReturnFrames = CCArray::create();
    pHeroDodgeFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    pHeroDodgeFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    for(int i=1; i<=9; i++) {
        pHeroDodgeFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("hero_hide_%d.png",10-i)->getCString()));
    }
    for(int i=1; i<=9; i++) {
        pHeroDodgeReturnFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("hero_hide_%d.png",i)->getCString()));
    }
    pHeroDodgeReturnFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    pHeroDodgeReturnFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    act_dodge = CCSequence::create(CCSequence::create(CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroDodgeFrames, GAME_FRAME_SPEED)),
                                                                      CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                                                         CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                                                            CCPlace::create(HERO_HIDE_DEST_POS)))),
                                                      CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroDodgeReturnFrames, GAME_FRAME_SPEED)),
                                                                      CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                                                         CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                                                            CCPlace::create(HERO_INIT_POS))))),
                                   CCCallFunc::create(this, callfunc_selector(SGHero::func_wating)));
    act_dodge->retain();
    pHeroDodgeFrames->release();
    pHeroDodgeReturnFrames->release();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    CCArray* pHeroActBlockFrames = CCArray::create();
	for(int i=1; i<=4; i++) {
		pHeroActBlockFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                            CCString::createWithFormat("hero_block_%d.png",i)->getCString()));
	}
    for(int i=3; i>=2; i--) {
		pHeroActBlockFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                            CCString::createWithFormat("hero_block_%d.png",i)->getCString()));
	}
    act_block = CCSequence::create(CCAnimate::create(CCAnimation::create(pHeroActBlockFrames, GAME_FRAME_SPEED)),
                                   CCCallFunc::create(this, callfunc_selector(SGHero::func_wating)));
	act_block->retain();
    pHeroActBlockFrames->release();
    
	CCArray* pHeroActDefendFrames = CCArray::create();
	for(int i=1; i<=3; i++) {
		pHeroActDefendFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                             CCString::createWithFormat("hero_defend_%d.png",i)->getCString()));
	}
    act_defend = CCSequence::create(CCAnimate::create(CCAnimation::create(pHeroActDefendFrames, GAME_FRAME_SPEED)),
                                    CCCallFunc::create(this, callfunc_selector(SGHero::func_wating)));
	act_defend->retain();
    pHeroActDefendFrames->release();
    
    CCArray* pHeroActDieFrames = CCArray::create();
	for(int i=1; i<=3; i++) {
		pHeroActDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                          CCString::createWithFormat("hero_defend_%d.png",i)->getCString()));
	}
    for(int i=1; i<=6; i++) {
		pHeroActDieFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                          CCString::createWithFormat("hero_die_%d.png",i)->getCString()));
	}
    //act_die = CCSequence::create(CCCallFunc::create(this, callfunc_selector(SGHero::func_stop)),
    //                             CCAnimate::create(CCAnimation::create(pHeroActDieFrames, GAME_FRAME_SPEED)));
    act_die = CCAnimate::create(CCAnimation::create(pHeroActDieFrames, GAME_FRAME_SPEED));
	act_die->retain();
    pHeroActDieFrames->release();
    
	CCArray* pHeroActAttackFrames = CCArray::create();
	for(int i=1; i<=9; i++) {
		pHeroActAttackFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                             CCString::createWithFormat("hero_attack_%d.png",i)->getCString()));
	}
    act_attack = CCSequence::create(CCAnimate::create(CCAnimation::create(pHeroActAttackFrames, GAME_FRAME_SPEED/1.5f)),
                                    CCCallFunc::create(this, callfunc_selector(SGHero::func_wating)));
	act_attack->retain();
    pHeroActAttackFrames->release();
    
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
    act_die->release();
    act_defend->release();
    act_attack->release();
    act_block->release();
    act_wait->release();
    
    this->release();
}

SGHero* SGHero::create(CCLayer* parent) {
    
    return (new SGHero(parent));
}