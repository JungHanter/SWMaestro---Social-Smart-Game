//
//  TouchInputTestScene.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 17..
//
//

#ifndef SketchGame_TouchInput_TouchInputTestScene_h
#define SketchGame_TouchInput_TouchInputTestScene_h

#include "cocos2d.h"


class TouchInputTestScene : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void ccTouchesCancelled(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void dragInputEnded(int dragDirection);
    
    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(TouchInputTestScene);
    
private:
    cocos2d::CCSprite* icon;
    cocos2d::CCPoint oldTouchPoint;
    bool bTouching;
    enum DragDirection { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
};

#endif
