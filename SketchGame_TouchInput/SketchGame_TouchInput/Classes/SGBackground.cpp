//
//  SGBackground.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#include "SGBackground.h"
#include "defines.h"

SGBackground* SGBackground::sharedSGBackground = NULL;

void SGBackground::pauseAllBackground() {
    bg_map->pauseSchedulerAndActions();
    bg_mountain->pauseSchedulerAndActions();
    bg_mountain2->pauseSchedulerAndActions();
}

void SGBackground::resumeAllBackground() {
    bg_map->resumeSchedulerAndActions();
    bg_mountain->resumeSchedulerAndActions();
    bg_mountain2->resumeSchedulerAndActions();
}

void SGBackground::func_mountainMove() {
    //bg_mountain->stopAllActions();
    bg_mountain->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN*2, -140.0f),
                                                                      CCRotateTo::create(0, 70.0f))));
}

void SGBackground::func_cloudMove() {
    CCSize cloudSize = bg_cloud->boundingBox().size;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //bg_cloud->stopAllActions();
    bg_cloud->runAction(CCRepeatForever::create(CCSequence::create(CCPlace::create(ccp(cloudSize.width, winSize.height)),
                                                                   CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD*2, ccp(-cloudSize.width*2,0)))));
}

SGBackground* SGBackground::sharedInstance(CCLayer* parent) {
    if(sharedSGBackground==NULL) {
        sharedSGBackground = new SGBackground(parent);
    }
    return sharedSGBackground;
}

SGBackground::SGBackground(CCLayer* parent) : CCObject::CCObject(), parentLayer(parent) {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //언덕 배경 로드
    pSpriteFrameCache->addSpriteFramesWithFile("bg_map_hill.plist", "bg_map_hill.png");
    bg_map = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_map_hill_1.png"));
    bg_map->retain();
    bg_map->setAnchorPoint(ccp(0, 0));
    bg_map->setPosition(ccp(0, 0));
    parentLayer->addChild(bg_map, ORDER_BACKGROUND, TAG_TEXTURE_BACKGROUND);
    CCArray* pBackgroundMapFrames = CCArray::create();
    for(int i=1; i<=10; i++) {
        pBackgroundMapFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                             CCString::createWithFormat("bg_map_hill_%d.png", i)->getCString()));
    }
    bg_map_action = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pBackgroundMapFrames, GAME_FRAME_SPEED)));
    bg_map_action->retain();
    bg_map->runAction(bg_map_action);
    pBackgroundMapFrames->release();
    
    //산, 구름 로드
    pSpriteFrameCache->addSpriteFramesWithFile("bg_etc.plist", "bg_etc.png");
    bg_mountain = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain->retain();
    bg_mountain->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain->setPosition(ccp(winSize.width/2, -440));
    parentLayer->addChild(bg_mountain, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
    bg_mountain2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain2->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain2->setPosition(ccp(winSize.width/2, -440));
    bg_mountain2->setRotation(70.0f);
    parentLayer->addChild(bg_mountain2, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
    bg_mountain->runAction(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN, -70.0f),
                                              CCSequence::create(CCRotateTo::create(0, 70.0f), CCCallFunc::create(this, callfunc_selector(SGBackground::func_mountainMove)))));
    bg_mountain2->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN*2, -140.0f),
                                                                       CCRotateTo::create(0, 70.0f))));
    
    bg_cloud = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_cloud.png"));
    bg_cloud->retain();
    bg_cloud->setAnchorPoint(ccp(0.0f, 1.0f));
    bg_cloud->setPosition(ccp(0, winSize.height));
    parentLayer->addChild(bg_cloud, ORDER_FARAWAY_CLOUD, TAG_TEXTURE_BACKGROUND);
    CCSize cloudSize = bg_cloud->boundingBox().size;
    bg_cloud2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_cloud.png"));
    bg_cloud2->retain();
    bg_cloud2->setAnchorPoint(ccp(0.0f, 1.0f));
    bg_cloud2->setPosition(ccp(cloudSize.width, winSize.height+15));
    parentLayer->addChild(bg_cloud2, ORDER_FARAWAY_CLOUD, TAG_TEXTURE_BACKGROUND);
    bg_cloud->runAction(CCSequence::create(CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD, ccp(-cloudSize.width,0)),
                                           CCSequence::create(CCMoveBy::create(0, ccp(cloudSize.width*2, 0)), CCCallFunc::create(this, callfunc_selector(SGBackground::func_cloudMove)))));
    //this->scheduleOnce(schedule_selector(SketchGameLayer::func_cloudMove), GAME_FRAME_SPEED*SPEED_CLOUD);
    bg_cloud2->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD*2, ccp(-cloudSize.width*2,0)),
                                                                    CCPlace::create(ccp(cloudSize.width, winSize.height+15)))));
    
    //성 로드
    bg_castle = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_castle_1.png"));
    bg_castle->retain();
    bg_castle->setPosition(ccp(290,240));
    parentLayer->addChild(bg_castle, ORDER_FARWAY_CASTLE, TAG_TEXTURE_BACKGROUND);
    CCArray* pCastleFrames = CCArray::create();
    for(int i=1; i<=3; i++) {
        pCastleFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_castle_%d.png", i)->getCString()));
    }
    bg_castle->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pCastleFrames, GAME_FRAME_SPEED))));
    pCastleFrames->release();
    
    this->retain();
}

SGBackground::~SGBackground() {
    CCObject::~CCObject();
    
    bg_map->release(); bg_map=NULL;
    bg_map_action->release(); bg_map_action=NULL;
    bg_mountain->release(); bg_mountain=NULL;
    bg_mountain2->release(); bg_mountain2=NULL;
    bg_cloud->release(); bg_cloud=NULL;
    bg_cloud2->release(); bg_cloud2=NULL;
    bg_castle->release(); bg_castle=NULL;
    
    this->release();
    
    sharedSGBackground = NULL;
}