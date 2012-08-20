//
//  TouchInputLayer.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#include "TouchInputLayer.h"

using namespace cocos2d;

void TouchInputLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = true;
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    oldTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    CCLog("touchBegan");
    
    return CCLayerColor::ccTouchesBegan(pTouches, pEvent);
}

void TouchInputLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
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

void TouchInputLayer::ccTouchesCancelled(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = false;
}

void TouchInputLayer::tapInputEnded(const CCPoint& point) {
    
}

void TouchInputLayer::dragInputEnded(const int dragDirection) {
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

bool TouchInputLayer::init() {
    //////////////////////////////
    //super init first
    CCLog("TouchInputLayerInit");
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,0)) )
    {
        return false;
    }
    
    //touch set
    this->setTouchEnabled(true);
    
    
    /////////////////////////////
    //codes below...
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    bTouching = false;
    
    return true;
}