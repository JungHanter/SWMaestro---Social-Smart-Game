//
//  TouchInputTestScene.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 17..
//
//

#include "TouchInputTestScene.h"

USING_NS_CC;




void TouchInputTestScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = true;
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    oldTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    CCLog("touchBegan");
    
    return CCLayerColor::ccTouchesBegan(pTouches, pEvent);
}

void TouchInputTestScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    const CCPoint nowTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    
    if(bTouching) {
        const CCFloat dist = 10.0f;
        if(ccpDistance(oldTouchPoint, nowTouchPoint) < dist) {
            //touch
            icon->setPosition(oldTouchPoint);
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

void TouchInputTestScene::ccTouchesCancelled(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = false;
}

void TouchInputTestScene::dragInputEnded(int dragDirection) {
    switch (dragDirection) {
        case DIR_UP:
            icon->runAction(CCEaseInOut::create(CCMoveBy::create(0.2f, ccp(0,20)), 1.0f));
            break;
            
        case DIR_DOWN:
            icon->runAction(CCEaseInOut::create(CCMoveBy::create(0.2f, ccp(0,-20)), 1.0f));
            break;
            
        case DIR_LEFT:
            icon->runAction(CCEaseInOut::create(CCMoveBy::create(0.2f, ccp(-20,0)), 1.0f));
            break;
            
        case DIR_RIGHT:
            icon->runAction(CCEaseInOut::create(CCMoveBy::create(0.2f, ccp(20,0)), 1.0f));
            break;
    }
}

// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
bool TouchInputTestScene::init() {
    //////////////////////////////
    // 1. super init first
    CCLog("app init");
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        return false;
    }
    
    //touch
    this->setTouchEnabled(true);
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(TouchInputTestScene::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    bTouching = false;
 
    icon = CCSprite::create("icon.png");
    icon->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(icon);
    
    return true;
}

// there's no 'id' in cpp, so we recommand to return the exactly class pointer
CCScene* TouchInputTestScene::scene() {
    CCScene *scene = CCScene::create();
    
    TouchInputTestScene *layer = TouchInputTestScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

// a selector callback
void TouchInputTestScene::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}