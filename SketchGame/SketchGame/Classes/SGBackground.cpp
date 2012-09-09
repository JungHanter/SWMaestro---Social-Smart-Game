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
	else if(parent != sharedSGBackground->parentLayer){
		parent->addChild(
			sharedSGBackground->bg_map,
			ORDER_BACKGROUND, TAG_TEXTURE_BACKGROUND);
		parent->addChild(
			sharedSGBackground->bg_cloud,
			ORDER_FARAWAY_CLOUD, TAG_TEXTURE_BACKGROUND);
		parent->addChild(
			sharedSGBackground->bg_mountain,
			ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
		parent->addChild(
			sharedSGBackground->bg_mountain2,
			ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE_BACKGROUND);
		parent->addChild(
			sharedSGBackground->bg_castle,
			ORDER_FARWAY_CASTLE, TAG_TEXTURE_BACKGROUND);
	}
    return sharedSGBackground;
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
    bg_map->runAction(bg_map_action);
    pBackgroundMapFrames->release();
    
    //overlay
    bg_overlay = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_forest_into_1.png"));
    bg_overlay->retain();
    bg_overlay->setAnchorPoint(ccp(0,0));
    bg_overlay->setPosition(ccp(0,0));
    parentLayer->addChild(bg_overlay, ORDER_BACKGROUND_OVERLAY, TAG_TEXTURE_BACKGROUND);
    
    //forest into
    CCArray* pForestIntoFrames = CCArray::create();
    for(int i=1; i<=15; i++) {
        pForestIntoFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                                                                          CCString::createWithFormat("bg_forest_into_%d.png", i)->getCString()));
    }
    
    //Mountain
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
    
    this->autorelease();
    this->retain();
}

SGBackground::~SGBackground() {
    
}

void SGBackground::releaseInstance(){
	SGBackground *background = 
		SGBackground::sharedInstance(NULL);
	background->bg_map->release(); background->bg_map=NULL;
    background->bg_map_action->release(); background->bg_map_action=NULL;
    background->bg_mountain->release(); background->bg_mountain=NULL;
    background->bg_mountain2->release(); background->bg_mountain2=NULL;
    background->bg_cloud->release(); background->bg_cloud=NULL;
    background->bg_cloud2->release(); background->bg_cloud2=NULL;
    background->bg_castle->release(); background->bg_castle=NULL;
    
    background->release();
    
    sharedSGBackground = NULL;
}