//
//  SketchGameScene.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#include "SketchGameScene.h"
#include "TouchInputLayer.h"

using namespace cocos2d;

////////////////////////////SktechGameLayer class////////////////////////

////////public method////////








void SketchGameLayer::tapInputEnded(const CCPoint& point) {
    if(CCRect::CCRectContainsPoint(obj_stone->boundingBox(), point)) {
        //CCLog("tapped Stone");
        //케릭터가 달리는 중이고, 캐릭터가 바위에 숨을 수 있는 위치 전까지만 터치 입력을 받음
        if(gameState == GAMESTATE_RUNNING &&
           obj_stone->getPosition().x > HERO_HIDE_ABLE_POS.x &&
           obj_stone->getPosition().y > HERO_HIDE_ABLE_POS.y) {
                CCLog("hero hide");
                hideHeroObject(true);
        }
    }
}

void SketchGameLayer::dragInputEnded(const int dragDirection) {
    switch (dragDirection) {
        case DIR_UP:
            break;
            
        case DIR_DOWN:
            break;
            
        case DIR_LEFT:
            break;
            
        case DIR_RIGHT:
            break;
    }
}

////////private method////////
void SketchGameLayer::hideHeroObject(bool bStone) {
    gameState = GAMESTATE_HIDE;
    this->schedule(schedule_selector(SketchGameLayer::func_startheroHide));
}

void SketchGameLayer::func_startheroHide() {
    //이 지점부터 케릭터가 움직인다
    if(obj_stone->getPosition().x <= HERO_HIDE_ABLE_POS.x
       && obj_stone->getPosition().y <= HERO_HIDE_ABLE_POS.y) {
        this->unschedule(schedule_selector(SketchGameLayer::func_startheroHide));
        hero->stopAllActions();
        hero->runAction(CCSpawn::create(hero_act_hide_stone, CCMoveTo::create(GAME_FRAME_SPEED*5, HERO_HIDE_DEST_POS)));
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveHide), GAME_FRAME_SPEED*5);
    }
}

void SketchGameLayer::func_heroMoveHide() {
    pauseAllBackground();
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*10);
}

void SketchGameLayer::func_heroMoveShow() {
    resumeAllBackground();
    hero->stopAllActions();
    hero->runAction(CCSequence::create(CCSpawn::create(hero_act_hide_stone, CCMoveTo::create(GAME_FRAME_SPEED*5, HERO_INIT_POS)),
                                       hero_act_run));
    gameState = GAMESTATE_RUNNING;
}

void SketchGameLayer::loadGameTexture() {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //언덕 배경 로드
    pSpriteFrameCache->addSpriteFramesWithFile("background_forest.plist", "background_forest.png");
    bg_map = CCSprite::create(pSpriteFrameCache->spriteFrameByName("background_forest_1.png"));
    bg_map->retain();
    bg_map->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(bg_map, ORDER_BACKGROUND, TAG_TEXTURE);
    CCArray* pBackgroundMapFrames = CCArray::create();
    for(int i=1; i<=5; i++) {
        pBackgroundMapFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("background_forest_%d.png", i)->getCString()));
    }
    bg_map_action = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pBackgroundMapFrames, GAME_FRAME_SPEED)));
    bg_map_action->retain();
    bg_map->runAction(bg_map_action);
    pBackgroundMapFrames->release();
    
    //산, 구름 로드
    pSpriteFrameCache->addSpriteFramesWithFile("bg_faraway.plist", "bg_faraway.png");
    bg_mountain = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain->retain();
    bg_mountain->setAnchorPoint(ccp(0.5f, 0.052f)); //0.052 = (298-267)/596
    bg_mountain->setPosition(ccp(winSize.width/2, -23-267));
    this->addChild(bg_mountain, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE);
    bg_mountain2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain2->setAnchorPoint(ccp(0.5f, 0.052f)); //0.052 = (298-267)/596
    bg_mountain2->setPosition(ccp(winSize.width/2, -23-267));
    bg_mountain2->setRotation(180.0f);
    this->addChild(bg_mountain2, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE);
    bg_mountain->runAction(CCRepeatForever::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN, -180.0f)));
    bg_mountain2->runAction(CCRepeatForever::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN, -180.0f)));
    
    bg_cloud = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_cloud.png"));
    bg_cloud->retain();
    bg_cloud->setAnchorPoint(ccp(0.0f, 1.0f));
    bg_cloud->setPosition(ccp(0, winSize.height));
    this->addChild(bg_cloud, ORDER_FARAWAY_CLOUD, TAG_TEXTURE);
    CCSize cloudSize = bg_cloud->boundingBox().size;
    bg_cloud2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_cloud.png"));
    bg_cloud2->retain();
    bg_cloud2->setAnchorPoint(ccp(0.0f, 1.0f));
    bg_cloud2->setPosition(ccp(cloudSize.width, winSize.height));
    this->addChild(bg_cloud2, ORDER_FARAWAY_CLOUD, TAG_TEXTURE);
    bg_cloud->runAction(CCSequence::create(CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD, ccp(-cloudSize.width,0)),
                                           CCMoveBy::create(0, ccp(cloudSize.width*2, 0))));
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_cloudMove), GAME_FRAME_SPEED*SPEED_CLOUD);
    bg_cloud2->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD*2, ccp(-cloudSize.width*2,0)),
                                                                    CCPlace::create(ccp(cloudSize.width, winSize.height)))));
    
    
    //오브제(풀, 바위) 로드
    pSpriteFrameCache->addSpriteFramesWithFile("obj_stone.plist", "obj_stone.png");
    obj_stone = CCSprite::create(pSpriteFrameCache->spriteFrameByName("obj_stone_1.png"));
    obj_stone->retain();
    //obj_stone->setPosition(ccp(243, 320-142));
    obj_stone->setPosition(ccp(-100,-100));
    this->addChild(obj_stone, ORDER_OBJECT, TAG_TEXTURE);
    CCArray* pStoneFrames = CCArray::create();
    CCArray* pStoneMoveActions = CCArray::create();
    for(int i=1; i<=17; i++) {
        pStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("obj_stone_%d.png",i)->getCString()));
    }
    //일단 테스트코드
    pStoneMoveActions->addObject(CCPlace::create(ccp(243, 320-142)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(236, 320-150)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(227, 320-155)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(213, 320-160)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(203, 320-166)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(195, 320-170)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(185, 320-183)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(173, 320-194)));   //ccp(173, 320-194) == HERO_HIDE_ABLE_POS
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(161, 320-206)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(151, 320-217)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(144, 320-226)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(139, 320-235)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(134, 320-241)));   //ccp(134, 320-241) == HERO_HIDE_DEST_POS
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(127, 320-252)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(120, 320-264)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(113, 320-281)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(105, 320-309)));
    pStoneMoveActions->addObject(CCDelayTime::create(GAME_FRAME_SPEED));
    pStoneMoveActions->addObject(CCPlace::create(ccp(-100, -100)));
    
    obj_stone_action = CCSpawn::create(CCAnimate::create(CCAnimation::create(pStoneFrames, GAME_FRAME_SPEED)),
                                       CCSequence::create(pStoneMoveActions));
    obj_stone_action->retain();
    pStoneFrames->release();
    pStoneMoveActions->release();
    this->schedule(schedule_selector(SketchGameLayer::logic_createObject), GAME_FRAME_SPEED*5.0f*4.0f);
    
    //캐릭터 로드
    pSpriteFrameCache->addSpriteFramesWithFile("hero_run.plist", "hero_run.png");
    hero = CCSprite::create(pSpriteFrameCache->spriteFrameByName("hero_run_1.png"));
    hero->retain();
    hero->setPosition(HERO_INIT_POS);
    this->addChild(hero, ORDER_HERO, TAG_TEXTURE);
    CCArray* pHeroActRunFrames = CCArray::create();
    for(int i=1; i<=4; i++) {
        pHeroActRunFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_run_%d.png",i)->getCString()));
    }
    hero_act_run = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pHeroActRunFrames, GAME_FRAME_SPEED)));
    hero_act_run->retain();
    hero->runAction(hero_act_run);
    pHeroActRunFrames->release();
    CCArray* pHeroActHideStoneFrames = CCArray::create();
    for(int i=1; i<=4; i++) {
        pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_run_%d.png",i)->getCString()));
    }
    hero_act_hide_stone = CCAnimate::create(CCAnimation::create(pHeroActHideStoneFrames, GAME_FRAME_SPEED));
    hero_act_hide_stone->retain();
    pHeroActHideStoneFrames->release();
    
    
    this->schedule(schedule_selector(SketchGameLayer::logic_printGameinfo));
}
 

void SketchGameLayer::func_cloudMove() {
    CCSize cloudSize = bg_cloud->boundingBox().size;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bg_cloud->stopAllActions();
    bg_cloud->runAction(CCRepeatForever::create(CCSequence::create(CCPlace::create(ccp(cloudSize.width, winSize.height)),
                                                                   CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD*2, ccp(-cloudSize.width*2,0)))));
}

void SketchGameLayer::logic_createObject() {
    if(gameState != GAMESTATE_RUNNING) return;
    
    int rnd = (int)(CCRANDOM_0_1()*3);
    if(rnd%3==0) {
        CCLog("Create Stone");
        obj_stone->runAction(obj_stone_action);
    } else {
        CCLog("Waiting Stone by num %d", rnd);
    }
}

void SketchGameLayer::logic_printGameinfo() {
    /*CCLog("stone position:(%d, %d) size:(%d, %d)", (int)(obj_stone->getPosition().x), (int)(obj_stone->getPosition().y),
          (int)(obj_stone->boundingBox().size.width), (int)(obj_stone->boundingBox().size.width));*/
    /*CCLog("cloud pos : 1(%d, %d) / 2(%d, %d)", (int)(bg_cloud->getPositionX()), (int)(bg_cloud->getPositionY()),
          (int)(bg_cloud2->getPositionX()), (int)(bg_cloud2->getPositionY()));*/
}

void SketchGameLayer::unloadGameTexture() {
    this->removeChildByTag(TAG_TEXTURE, true);
    bg_map->release(); bg_map=NULL;
    bg_map_action->release(); bg_map_action=NULL;
    obj_stone->release(); obj_stone=NULL;
    obj_stone_action->release(); obj_stone_action=NULL;
    bg_mountain->release(); bg_mountain=NULL;
    bg_mountain2->release(); bg_mountain2=NULL;
    bg_cloud->release(); bg_cloud=NULL;
    bg_cloud2->release(); bg_cloud2=NULL;
    hero->release(); hero=NULL;
    hero_act_run->release(); hero_act_run=NULL;
    hero_act_hide_stone->release(); hero_act_hide_stone=NULL;
    
    CCSpriteFrameCache *pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCTextureCache *pTextureCahce = CCTextureCache::sharedTextureCache();
    
    pFrameCache->removeSpriteFramesFromFile("backgroud_forest.plist");
    pTextureCahce->removeTextureForKey("background_forest.png");
    
    //CCTextureCache::sharedTextureCache()->removeAllTextures();
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

void SketchGameLayer::pauseAllBackground() {
    bg_map->pauseSchedulerAndActions();
    bg_mountain->pauseSchedulerAndActions();
    bg_mountain2->pauseSchedulerAndActions();
    //bg_cloud->pauseSchedulerAndActions();
    //bg_cloud2->pauseSchedulerAndActions();
    obj_stone->pauseSchedulerAndActions();
}

void SketchGameLayer::resumeAllBackground() {
    bg_map->resumeSchedulerAndActions();
    bg_mountain->resumeSchedulerAndActions();
    bg_mountain2->resumeSchedulerAndActions();
    //bg_cloud->resumeSchedulerAndActions();
    //bg_cloud2->resumeSchedulerAndActions();
    obj_stone->resumeSchedulerAndActions();
}

////////init method////////
bool SketchGameLayer::init() {
    if( CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->_label = CCLabelTTF::create("", "Artial", 32);
        _label->retain();
        //_label->setColor( ccc3(0,0,0) );
        //_label->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(_label);
        
        this->setTouchEnabled(true);
        
        //user codes
        bTouching = false;
        gameState = GAMESTATE_RUNNING;
        
        loadGameTexture();
        
        return true;
    } else {
        return false;
    }
}

SketchGameLayer::~SketchGameLayer() {
    unloadGameTexture();
    
    if (_label)
    {
        _label->release();
        _label = NULL;
    }
}

////////touch method///////
void SketchGameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = true;
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    oldTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    
    return CCLayerColor::ccTouchesBegan(pTouches, pEvent);
}

void SketchGameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    const CCPoint nowTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    
    if(bTouching) {
        const CCFloat dist = 10.0f;
        if(ccpDistance(oldTouchPoint, nowTouchPoint) < dist) {
            //tap
            tapInputEnded(nowTouchPoint);
        } else {
            //drag
            int dragDirection;
            
            CCPoint aux = ccpNormalize(ccp(nowTouchPoint.x - oldTouchPoint.x,
                                           nowTouchPoint.y - oldTouchPoint.y));
            if(fabsl(aux.x) >= fabsl(aux.y)) {
                if(aux.x>=0) dragDirection = DIR_RIGHT;
                else dragDirection = DIR_LEFT;
            } else {
                if(aux.y>=0) dragDirection = DIR_UP;
                else dragDirection = DIR_DOWN;
            }
            
            dragInputEnded(dragDirection);
        }
    }
    
    bTouching = false;
}

void SketchGameLayer::ccTouchesCancelled(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = false;
}


////////////////////////////SktechGameScene class////////////////////////

bool SketchGameScene::init() {
    if( CCScene::init() )
    {
        //GameOverLayer::node() is deprecated
        this->_layer = SketchGameLayer::create();
        this->_layer->retain();
        this->addChild(_layer, 0);
        
        return true;
    } else {
        return false;
    }
}

SketchGameScene::~SketchGameScene() {
    if (_layer)
    {
        _layer->release();
        _layer = NULL;
    }    
}

