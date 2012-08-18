//
//  SketchGameScene.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#include "SketchGameScene.h"

using namespace cocos2d;

////////////////////////////SktechGameLayer class////////////////////////

////////public method////////




////////private method////////
void SketchGameLayer::loadGameTexture() {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //언덕 배경 로드
    pSpriteFrameCache->addSpriteFramesWithFile("background_forest.plist", "background_forest.png");
    bg_map = CCSprite::create(pSpriteFrameCache->spriteFrameByName("background_forest_1.png"));
    bg_map->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(bg_map);
    CCArray* pBackgroundMapFrames = CCArray::create();
    for(int i=1; i<=5; i++) {
        pBackgroundMapFrames->addObject(pSpriteFrameCache->spriteFrameByName(
            CCString::createWithFormat("background_forest_%d.png", i)->getCString()));
    }
    bg_map_action = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pBackgroundMapFrames, 0.15f)));
    bg_map->runAction(bg_map_action);
}

void SketchGameLayer::unloadGameTexture() {
    
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
        
        //user codes
        loadGameTexture();
        
        return true;
    } else {
        return false;
    }
}

SketchGameLayer::~SketchGameLayer() {
    if (_label)
    {
        _label->release();
        _label = NULL;
    }
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

