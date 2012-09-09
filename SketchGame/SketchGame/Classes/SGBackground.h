//
//  SGBackground.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 9. 1..
//
//

#ifndef __SketchGame_TouchInput__SGBackground__
#define __SketchGame_TouchInput__SGBackground__

#include "cocos2d.h"
using namespace cocos2d;

class SGBackground : public CCObject {

public:
    void pauseAllBackground();
    void resumeAllBackground();
    
    void gameStart();

	CCLayer* parentLayer;
private:
    cocos2d::CCSprite* bg_map;
    cocos2d::CCActionInterval* bg_map_action;
    
    CCSprite* bg_overlay;
    CCActionInterval* bg_forest_action, *bg_forest_into_action;
    CCActionInterval* bg_cave_action, *bg_cave_into_action, *bg_cave_out_action;
    
    cocos2d::CCSprite* bg_mountain, *bg_mountain2;
    cocos2d::CCSprite* bg_cloud, *bg_cloud2;
    cocos2d::CCSprite* bg_castle;
    
    
    
private:
    void func_mountainMove();
    void func_cloudMove();
    
private:
    SGBackground(CCLayer* parent);
    static SGBackground* sharedSGBackground;
    virtual ~SGBackground();
    void resetParent(CCLayer* parent);

public:
    
    static SGBackground* sharedInstance(CCLayer *parent);
};

#endif /* defined(__SketchGame_TouchInput__SGBackground__) */
