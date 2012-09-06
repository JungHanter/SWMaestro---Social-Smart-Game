//
//  SGHidableObject.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#ifndef __SketchGame_TouchInput__SGHidableObject__
#define __SketchGame_TouchInput__SGHidableObject__

#include "cocos2d.h"
using namespace cocos2d;

class SGHidableObject : public CCObject {
    
public:
    void pause();
    void resume();
    
private:
    cocos2d::CCSprite* object;
    cocos2d::CCActionInterval* moveAction;
    
    CCLayer* const parentLayer;
    
private:
    void func_mountainMove();
    void func_cloudMove();
    
public:
    SGHidableObject(CCLayer* parent);
    virtual ~SGHidableObject();
};

#endif /* defined(__SketchGame_TouchInput__SGHidableObject__) */
