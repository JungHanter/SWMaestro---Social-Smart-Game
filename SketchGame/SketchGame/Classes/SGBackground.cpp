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
    CCLog("pause bg");
    bg_map->pauseSchedulerAndActions();
    bg_mountain->pauseSchedulerAndActions();
    bg_mountain2->pauseSchedulerAndActions();
    
    if(bGameScene) {
        bg_overlay->pauseSchedulerAndActions();
    }
}

void SGBackground::resumeAllBackground() {
    bg_map->resumeSchedulerAndActions();
    bg_mountain->resumeSchedulerAndActions();
    bg_mountain2->resumeSchedulerAndActions();
    
    if(bGameScene) {
        bg_overlay->resumeSchedulerAndActions();
    }
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
	else if(parent != sharedSGBackground->parentLayer){
        //sharedSGBackground->release();
        //sharedSGBackground = new SGBackground(parent);
        sharedSGBackground->resetParent(parent);
	}
    return sharedSGBackground;
}

void SGBackground::resetParent(CCLayer* parent) {
    parentLayer->removeChild(bg_map, false);
    parentLayer->removeChild(bg_cloud, false);
    parentLayer->removeChild(bg_cloud2, false);
    parentLayer->removeChild(bg_mountain, false);
    parentLayer->removeChild(bg_mountain2, false);
    parentLayer->removeChild(bg_castle, false);
    parentLayer->removeChild(bg_overlay, false);
    parentLayer->removeChildByTag(TAG_TEXTURE_BACKGROUND, false);
    parentLayer->removeChild(this, false);
    parentLayer = parent;
    
    parent->addChild(bg_map,
                     ORDER_BACKGROUND, TAG_TEXTURE_BACKGROUND);
    parent->addChild(bg_cloud,
                     ORDER_FARAWAY_CLOUD, TAG_TEXTURE_BACKGROUND);
    parent->addChild(bg_cloud2,
                     ORDER_FARWAY_CASTLE, TAG_TEXTURE_BACKGROUND);
    parent->addChild(bg_mountain,
                     ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
    parent->addChild(bg_mountain2,
                     ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
    parent->addChild(bg_castle,
                     ORDER_FARWAY_CASTLE, TAG_TEXTURE_BACKGROUND);
    parent->addChild(bg_overlay,
                     ORDER_BACKGROUND_OVERLAY, TAG_TEXTURE_BACKGROUND);
    parent->addChild(this);
}

void SGBackground::gameStart() {
    bGameScene = true;
    
    overlayState = OVERLAY_NONE;
    //bg_overlay->schedule(schedule_selector(SGBackground::logic_overlay), OVERLAY_CYCLE);
    CCArray* pOverlayFrames = CCArray::create();
    pOverlayFrames->addObject(CCMoveTo::create(0.0f, ccp(-500,-500)));
    pOverlayFrames->addObject(CCDelayTime::create(GAME_FRAME_SPEED*30*4));
    pOverlayFrames->addObject(CCMoveTo::create(0.0f, ccp(0,0)));
    pOverlayFrames->addObject(bg_forest_into_action);
    pOverlayFrames->addObject(CCRepeat::create(bg_forest_action, 10));
    pOverlayFrames->addObject(bg_cave_into_action);
    pOverlayFrames->addObject(CCRepeat::create(bg_cave_action, 7));
    pOverlayFrames->addObject(bg_cave_out_action);
    
    bg_overlay->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(pOverlayFrames)));
    
    
    bg_map->runAction(bg_map_action);
    bg_mountain->runAction(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN, -70.0f),
                                              CCSequence::create(CCRotateTo::create(0, 70.0f), CCCallFunc::create(this, callfunc_selector(SGBackground::func_mountainMove)))));
    bg_mountain2->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN*2, -140.0f),
                                                                       CCRotateTo::create(0, 70.0f))));
    
}

void SGBackground::resetLogic() {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    bg_overlay->setPosition(ccp(-500,-500));
    bg_overlay->pauseSchedulerAndActions();
    bg_overlay->stopAllActions();
    //bg_overlay->unschedule(schedule_selector(SGBackground::logic_overlay));
    //bg_overlay->unscheduleAllSelectors();
    
    bg_map->stopAllActions();
    
    bg_mountain->stopAllActions();
    bg_mountain2->stopAllActions();
    bg_mountain->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain->setPosition(ccp(winSize.width/2, -440));
    bg_mountain2->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain2->setPosition(ccp(winSize.width/2, -440));
    bg_mountain2->setRotation(70.0f);
}

void SGBackground::logic_overlay(float) {
    //    bg_overlay->stopAllActions();
    CCLog("logic overlay = %d", overlayState);
    overlayState = 0;
    switch(overlayState) {
        case OVERLAY_NONE:
            bg_overlay->stopAction(bg_non_overlay);
            bg_overlay->runAction(CCSequence::create(bg_forest_into_action, CCCallFunc::create(this, callfunc_selector(SGBackground::run_overlay))));
            break;
            
        case OVERLAY_FOREST:
            bg_overlay->stopAction(bg_forest_action);
            bg_overlay->runAction(CCSequence::create(bg_cave_into_action, CCCallFunc::create(this, callfunc_selector(SGBackground::run_overlay))));
            break;
            
        case OVERLAY_CAVE:
            bg_overlay->stopAction(bg_cave_action);
            bg_overlay->runAction(CCSequence::create(bg_cave_out_action, CCCallFunc::create(this, callfunc_selector(SGBackground::run_overlay))));
            break;
    }
}

void SGBackground::run_overlay(){
    CCLog("lrun overlay");
    switch (overlayState) {
        case OVERLAY_NONE:
            bg_overlay->runAction(bg_forest_action);
            overlayState = OVERLAY_FOREST;
            break;
            
        case OVERLAY_FOREST:
            bg_overlay->runAction(bg_cave_action);
            overlayState = OVERLAY_CAVE;
            break;
            
        case OVERLAY_CAVE:
            bg_overlay->runAction(bg_non_overlay);
            overlayState = OVERLAY_NONE;
            break;
    }
    
}

SGBackground::SGBackground(CCLayer* parent) : parentLayer(parent) {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
	pSpriteFrameCache->addSpriteFramesWithFile("bg_map_hill.plist", "bg_map_hill.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_map_hill2.plist", "bg_map_hill2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave1.plist", "bg_cave1.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave2.plist", "bg_cave2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_into1.plist", "bg_cave_into1.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_into2.plist", "bg_cave_into2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_into3.plist", "bg_cave_into3.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_out.plist", "bg_cave_out.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_out2.plist", "bg_cave_out2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest.plist", "bg_forest.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest_into.plist", "bg_forest_into.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest_into2.plist", "bg_forest_into2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest_additional.plist", "bg_forest_additional.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_etc.plist", "bg_etc.png");
    
    //map
    bg_map = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_map_hill_1.png"));
    bg_map->retain();
    bg_map->setAnchorPoint(ccp(0, 0));
    bg_map->setPosition(ccp(0, 0));
    parentLayer->addChild(bg_map, ORDER_BACKGROUND, TAG_TEXTURE_BACKGROUND);
    CCArray* pBackgroundMapFrames = CCArray::create();
    for(int i=1; i<=15; i++) {
        pBackgroundMapFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                             CCString::createWithFormat("bg_map_hill_%d.png", i)->getCString()));
    }
    bg_map_action = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pBackgroundMapFrames, GAME_FRAME_SPEED)));
    bg_map_action->retain();
    //bg_map->runAction(bg_map_action);
    pBackgroundMapFrames->release();
    
    //overlay
    bg_overlay = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_forest_into_1.png"));
    bg_overlay->retain();
    bg_overlay->setAnchorPoint(ccp(0,0));
    bg_overlay->setPosition(ccp(-500,-500));
    parentLayer->addChild(bg_overlay, ORDER_BACKGROUND_OVERLAY, TAG_TEXTURE_BACKGROUND);
    
    bg_non_overlay = CCPlace::create(ccp(-500,-500));
    
    CCArray* pForestFrames = CCArray::create();
    for(int i=1; i<=7; i++) {
        pForestFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_forest_%d.png", i)->getCString()));
    }
    //bg_forest_action = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pForestFrames, GAME_FRAME_SPEED)));
    bg_forest_action = CCAnimate::create(CCAnimation::create(pForestFrames, GAME_FRAME_SPEED));
    bg_forest_action->retain();
    pForestFrames->release();
    
    CCArray* pForestIntoFrames = CCArray::create();
    for(int i=1; i<=15; i++) {
        pForestIntoFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_forest_into_%d.png", i)->getCString()));
    }
    bg_forest_into_action = CCAnimate::create(CCAnimation::create(pForestIntoFrames, GAME_FRAME_SPEED));
    bg_forest_into_action->retain();
    pForestIntoFrames->release();
    
    CCArray* pCaveIntoFrames = CCArray::create();
    for(int i=1; i<=15; i++) {
        pCaveIntoFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_cave_into_%d.png", i)->getCString()));
    }
    //bg_cave_action = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pCaveIntoFrames, GAME_FRAME_SPEED)));
    bg_cave_into_action = CCAnimate::create(CCAnimation::create(pCaveIntoFrames, GAME_FRAME_SPEED));
    bg_cave_into_action->retain();
    pCaveIntoFrames->release();
    
    CCArray* pCaveFrames = CCArray::create();
    for(int i=1; i<=12; i++) {
        pCaveFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_cave_%d.png", i)->getCString()));
    }
    bg_cave_action = CCAnimate::create(CCAnimation::create(pCaveFrames, GAME_FRAME_SPEED));
    bg_cave_action->retain();
    pCaveFrames->release();
    
    CCArray* pCaveOutFrames = CCArray::create();
    for(int i=1; i<=11; i++) {
        pCaveOutFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_cave_out_%d.png", i)->getCString()));
    }
    bg_cave_out_action = CCAnimate::create(CCAnimation::create(pCaveOutFrames, GAME_FRAME_SPEED));
    bg_cave_out_action->retain();
    pCaveOutFrames->release();
    
    overlayState = OVERLAY_NONE;
    
    
    //Mountain
    bg_mountain = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain->retain();
    bg_mountain->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain->setPosition(ccp(winSize.width/2, -440));
    parentLayer->addChild(bg_mountain, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
    bg_mountain2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain2->retain();
    bg_mountain2->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain2->setPosition(ccp(winSize.width/2, -440));
    bg_mountain2->setRotation(70.0f);
    parentLayer->addChild(bg_mountain2, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
    /*bg_mountain->runAction(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN, -70.0f),
     CCSequence::create(CCRotateTo::create(0, 70.0f), CCCallFunc::create(this, callfunc_selector(SGBackground::func_mountainMove)))));
     bg_mountain2->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN*2, -140.0f),
     CCRotateTo::create(0, 70.0f))));*/
    
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
    
    //castle
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
    
    bGameScene = false;
    
    parentLayer->addChild(this);
    this->autorelease();
    this->retain();
}

SGBackground::~SGBackground() {
	bg_map->release();bg_map=NULL;
    bg_map_action->release(); bg_map_action=NULL;
    bg_mountain->release(); bg_mountain=NULL;
    bg_mountain2->release(); bg_mountain2=NULL;
    bg_cloud->release(); bg_cloud=NULL;
    bg_cloud2->release(); bg_cloud2=NULL;
    bg_castle->release(); bg_castle=NULL;
    
    
    bg_overlay->release();
    bg_forest_action->release();
    bg_forest_into_action->release();
    bg_cave_action->release();
    bg_cave_into_action->release();
    bg_cave_out_action->release();
    bg_non_overlay->release();
    
    this->release();
    sharedSGBackground = NULL;
}
