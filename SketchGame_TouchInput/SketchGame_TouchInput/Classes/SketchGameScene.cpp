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
    this->schedule(schedule_selector(SketchGameLayer::func_startHeroHide));
}

void SketchGameLayer::func_startHeroHide() {
    //이 지점부터 케릭터가 움직인다
    if(obj_stone->getPosition().y <= HERO_HIDE_ABLE_POS.y) {
        this->unschedule(schedule_selector(SketchGameLayer::func_startHeroHide));
        pauseAllBackground();
        hero->stopAllActions();
        
        hero->runAction(hero_act_hide);
        //hero->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_HIDE_DEST_POS)));
        
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveHide), GAME_FRAME_SPEED*11);
    }
}

void SketchGameLayer::func_heroMoveHide() {
    //pauseAllBackground();
    hero->stopAllActions();
    hero->runAction(hero_act_keep);
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*8);
}

void SketchGameLayer::func_heroMoveShow() {
    hero->stopAllActions();
    
    hero->runAction(hero_act_show);
    //hero->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_INIT_POS)));
    
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_startHeroRun), GAME_FRAME_SPEED*11);
    gameState = GAMESTATE_RUNNING;
}

void SketchGameLayer::func_startHeroRun() {
    resumeAllBackground();
    hero->stopAllActions();
    hero->setPosition(HERO_INIT_POS);
    hero->runAction(hero_act_run);
}

void SketchGameLayer::loadGameTexture() {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //언덕 배경 로드
    pSpriteFrameCache->addSpriteFramesWithFile("bg_map_hill.plist", "bg_map_hill.png");
    bg_map = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_map_hill_1.png"));
    bg_map->retain();
    bg_map->setAnchorPoint(ccp(0, 0));
    bg_map->setPosition(ccp(0, 0));
    this->addChild(bg_map, ORDER_BACKGROUND, TAG_TEXTURE);
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
    this->addChild(bg_mountain, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE);
    bg_mountain2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_mountain.png"));
    bg_mountain2->setAnchorPoint(ccp(0.5f, MOUNTAIN_ANCOHR_Y));
    bg_mountain2->setPosition(ccp(winSize.width/2, -440));
    bg_mountain2->setRotation(70.0f);
    this->addChild(bg_mountain2, ORDER_FARAWAY_MOUNTAIN, TAG_TEXTURE);
    bg_mountain->runAction(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN, -70.0f),
                           CCSequence::create(CCRotateTo::create(0, 70.0f), CCCallFunc::create(this, callfunc_selector(SketchGameLayer::func_mountainMove)))));
    bg_mountain2->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN*2, -140.0f),
                                                                       CCRotateTo::create(0, 70.0f))));
    
    bg_cloud = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_cloud.png"));
    bg_cloud->retain();
    bg_cloud->setAnchorPoint(ccp(0.0f, 1.0f));
    bg_cloud->setPosition(ccp(0, winSize.height));
    this->addChild(bg_cloud, ORDER_FARAWAY_CLOUD, TAG_TEXTURE);
    CCSize cloudSize = bg_cloud->boundingBox().size;
    bg_cloud2 = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_cloud.png"));
    bg_cloud2->retain();
    bg_cloud2->setAnchorPoint(ccp(0.0f, 1.0f));
    bg_cloud2->setPosition(ccp(cloudSize.width, winSize.height+15));
    this->addChild(bg_cloud2, ORDER_FARAWAY_CLOUD, TAG_TEXTURE);
    bg_cloud->runAction(CCSequence::create(CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD, ccp(-cloudSize.width,0)),
                        CCSequence::create(CCMoveBy::create(0, ccp(cloudSize.width*2, 0)), CCCallFunc::create(this, callfunc_selector(SketchGameLayer::func_cloudMove)))));
    //this->scheduleOnce(schedule_selector(SketchGameLayer::func_cloudMove), GAME_FRAME_SPEED*SPEED_CLOUD);
    bg_cloud2->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD*2, ccp(-cloudSize.width*2,0)),
                                                                    CCPlace::create(ccp(cloudSize.width, winSize.height+15)))));
    
    //성 로드
    bg_castle = CCSprite::create(pSpriteFrameCache->spriteFrameByName("bg_castle_1.png"));
    bg_castle->retain();
    bg_castle->setPosition(ccp(290,240));
    this->addChild(bg_castle, ORDER_FARWAY_CASTLE, TAG_TEXTURE);
    CCArray* pCastleFrames = CCArray::create();
    for(int i=1; i<=3; i++) {
        pCastleFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("bg_castle_%d.png", i)->getCString()));
    }
    bg_castle->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pCastleFrames, GAME_FRAME_SPEED))));
    pCastleFrames->release();
    
    
    //오브제(풀, 바위) 로드
    const float coefficient = obj_coefficient(23,4);    //23등분일때 4배
    
    CCPoint arrPoint[23];
    //const float x_stone_add = 225/22, y_stone_add = 250/22;
    for(int i=0; i<23; i++) {
        //225,190 -> 0, -60
        //arrPoint[i].setPoint(225 - (x_stone_add*i), 190 - (y_stone_add*i));
        arrPoint[i].setPoint((78*(float)pow(23-i, coefficient))-78, (83*(float)pow(23-i, coefficient))-83-49);
        CCLog("%lf, %lf", arrPoint[i].x, arrPoint[i].y);
    }
    HERO_HIDE_ABLE_POS = arrPoint[15];
    
    //0.3 -> 1.2배 된다고 할 때
    float arrScaling[23];
    for(int i=0; i<23; i++) {
        //arrScaling[i] = 0.3f * (float)pow(i+1, coefficient);
        arrScaling[i] = 1.5f - ( 0.3f * (float)pow(23-i, coefficient) );
        //CCLog("i = %lf", arrScaling[i]);
    }
    
    obj_stone = CCSprite::create(pSpriteFrameCache->spriteFrameByName("obj_stone_1.png"));
    obj_stone->retain();
    obj_stone->setScale(arrScaling[0]);
    obj_stone->setPosition(ccp(-100,-100));
    this->addChild(obj_stone, ORDER_OBJECT, TAG_TEXTURE);
    CCArray* pStoneFrames = CCArray::create();
    CCArray* pStoneMoveActions = CCArray::create();
    for(int i=1; i<=4; i++) {
        pStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("obj_stone_%d.png",i)->getCString()));
    }

    //이건 자연스럽게 움직인거
    /*pStoneMoveActions->addObject(CCSpawn::create(CCMoveTo::create(0, ccp(225,190)),
                                                 CCScaleTo::create(0, OBJ_SCALING_F01)));
    pStoneMoveActions->addObject(CCSpawn::create(CCMoveTo::create(GAME_FRAME_SPEED*23, ccp(0,-60)),
                                                 CCScaleTo::create(GAME_FRAME_SPEED*23, OBJ_SCALING_F23)));*/

    //일단 테스트코드 나열한거
    /*pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[0]), CCScaleTo::create(0, OBJ_SCALING_F01))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[1]), CCScaleTo::create(0, OBJ_SCALING_F02))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[2]), CCScaleTo::create(0, OBJ_SCALING_F03))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[3]), CCScaleTo::create(0, OBJ_SCALING_F04))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[4]), CCScaleTo::create(0, OBJ_SCALING_F05))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[5]), CCScaleTo::create(0, OBJ_SCALING_F06))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[6]), CCScaleTo::create(0, OBJ_SCALING_F07))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[7]), CCScaleTo::create(0, OBJ_SCALING_F08))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[8]), CCScaleTo::create(0, OBJ_SCALING_F09))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[9]), CCScaleTo::create(0, OBJ_SCALING_F10))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[10]), CCScaleTo::create(0, OBJ_SCALING_F11))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[11]), CCScaleTo::create(0, OBJ_SCALING_F12))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[12]), CCScaleTo::create(0, OBJ_SCALING_F13))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[13]), CCScaleTo::create(0, OBJ_SCALING_F14))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[14]), CCScaleTo::create(0, OBJ_SCALING_F15))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[15]), CCScaleTo::create(0, OBJ_SCALING_F16))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[16]), CCScaleTo::create(0, OBJ_SCALING_F17))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[17]), CCScaleTo::create(0, OBJ_SCALING_F18))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[18]), CCScaleTo::create(0, OBJ_SCALING_F19))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[19]), CCScaleTo::create(0, OBJ_SCALING_F20))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[20]), CCScaleTo::create(0, OBJ_SCALING_F21))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[21]), CCScaleTo::create(0, OBJ_SCALING_F22))));
    pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                 CCSpawn::create(CCPlace::create(arrPoint[22]), CCScaleTo::create(0, OBJ_SCALING_F23))));*/
    
    //지수함수그래프 N등분일때 4배수 계수적용
    for(int i=0; i<23; i++) {
        pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                     CCSpawn::create(CCPlace::create(arrPoint[i]), CCScaleTo::create(0, arrScaling[i]))));
    }
    
    pStoneMoveActions->addObject(CCSpawn::create(CCPlace::create(ccp(-100,-100)), CCScaleTo::create(0, OBJ_SCALING_F01)));
    
    obj_stone_action = CCSpawn::create(CCRepeat::create(CCAnimate::create(CCAnimation::create(pStoneFrames, GAME_FRAME_SPEED)), 6),
                                       CCSequence::create(pStoneMoveActions));
    obj_stone_action->retain();
    pStoneFrames->release();
    pStoneMoveActions->release();
    this->schedule(schedule_selector(SketchGameLayer::logic_createObject), GAME_FRAME_SPEED*30.0f);
    
    
    //캐릭터 로드
    pSpriteFrameCache->addSpriteFramesWithFile("hero.plist", "hero.png");
    hero = CCSprite::create(pSpriteFrameCache->spriteFrameByName("hero_keep.png"));
    hero->retain();
    hero->setPosition(HERO_INIT_POS);
    this->addChild(hero, ORDER_HERO, TAG_TEXTURE);
    CCArray* pHeroActRunFrames = CCArray::create();
    for(int i=1; i<=7; i++) {
        pHeroActRunFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_walk_%d.png",i)->getCString()));
    }
    
    hero_act_run = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pHeroActRunFrames, GAME_FRAME_SPEED)));
    hero_act_run->retain();
    hero->runAction(hero_act_run);
    pHeroActRunFrames->release();
    
    CCArray* pHeroActKeepStoneFrames = CCArray::create();
    pHeroActKeepStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_keep.png"));
    hero_act_keep = CCAnimate::create(CCAnimation::create(pHeroActKeepStoneFrames));
    hero_act_keep->retain();
    pHeroActKeepStoneFrames->release();
    
    //hero->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_HIDE_DEST_POS)));
    //hero->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_INIT_POS)));
    CCArray* pHeroActHideStoneFrames = CCArray::create();
    pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    for(int i=1; i<=9; i++) {
        pHeroActHideStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_hide_%d.png",10-i)->getCString()));
    }
    hero_act_hide = CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActHideStoneFrames, GAME_FRAME_SPEED)),
                                    CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                      CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                         CCPlace::create(HERO_HIDE_DEST_POS))));
    hero_act_hide->retain();
    pHeroActHideStoneFrames->release();
    
    CCArray* pHeroActShowStoneFrames = CCArray::create();
    for(int i=1; i<=9; i++) {
        pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("hero_hide_%d.png",i)->getCString()));
    }
    pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_2.png"));
    pHeroActShowStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName("hero_hide_1.png"));
    hero_act_show = CCSpawn::create(CCAnimate::create(CCAnimation::create(pHeroActShowStoneFrames, GAME_FRAME_SPEED)),
                                    CCSequence::create(CCDelayTime::create(GAME_FRAME_SPEED*5),
                                                       CCSequence::create(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED), CCPlace::create(HERO_HIDE_MID_POS)),
                                                                          CCPlace::create(HERO_INIT_POS))));
    hero_act_show->retain();
    pHeroActShowStoneFrames->release();
    
    
    
    
    this->schedule(schedule_selector(SketchGameLayer::logic_printGameinfo));
}


void SketchGameLayer::func_mountainMove() {
    //bg_mountain->stopAllActions();
    bg_mountain->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(GAME_FRAME_SPEED*SPEED_MOUNTAIN*2, -140.0f),
                                                                       CCRotateTo::create(0, 70.0f))));
}

void SketchGameLayer::func_cloudMove() {
    CCSize cloudSize = bg_cloud->boundingBox().size;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //bg_cloud->stopAllActions();
    bg_cloud->runAction(CCRepeatForever::create(CCSequence::create(CCPlace::create(ccp(cloudSize.width, winSize.height)),
                                                                   CCMoveBy::create(GAME_FRAME_SPEED*SPEED_CLOUD*2, ccp(-cloudSize.width*2,0)))));
}

void SketchGameLayer::logic_createObject() {
    if(gameState != GAMESTATE_RUNNING) return;
    
    int rnd = (int)(CCRANDOM_0_1()*3);
    if(true){//if(rnd%3==0) {
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
    hero_act_hide->release(); hero_act_hide=NULL;
    
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

