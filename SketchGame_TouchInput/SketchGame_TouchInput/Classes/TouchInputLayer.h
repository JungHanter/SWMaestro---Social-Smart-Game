//
//  TouchInputLayer.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#ifndef __SketchGame_TouchInput__TouchInputLayer__
#define __SketchGame_TouchInput__TouchInputLayer__

#include "cocos2d.h"
#include "SketchGameScene.h"

class TouchInputLayer : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    LAYER_CREATE_FUNC(TouchInputLayer);
    
    //void setDragInputEnded(void (*dragInputEnded)(int));
    //void setTapInputEnded(void (*tapInputEnded)(cocos2d::CCPoint&));
    
protected:
    void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void ccTouchesCancelled(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void dragInputEnded(const int dragDirection);
    void tapInputEnded(const cocos2d::CCPoint& point);
    
private:
    cocos2d::CCPoint oldTouchPoint;
    bool bTouching;
    enum DragDirection { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
};

#endif /* defined(__SketchGame_TouchInput__TouchInputLayer__) */
