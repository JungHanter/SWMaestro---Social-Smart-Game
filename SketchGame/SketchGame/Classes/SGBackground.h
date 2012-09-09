<<<<<<< HEAD
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

	CCLayer* const parentLayer;
private:
    cocos2d::CCSprite* bg_map;
    cocos2d::CCActionInterval* bg_map_action;
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

public:
    
    static SGBackground* sharedInstance(CCLayer *parent);
	static void releaseInstance();
};

#endif /* defined(__SketchGame_TouchInput__SGBackground__) */
=======
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

private:
    CCSprite* bg_map;
    CCActionInterval* bg_map_action;
    
    CCSprite* bg_overlay;
    CCActionInterval* bg_forest_action, *bg_forest_into_action;
    CCActionInterval* bg_cave_action, *bg_cave_into_action, *bg_cave_out_action;
    
    CCSprite* bg_mountain, *bg_mountain2;
    CCSprite* bg_cloud, *bg_cloud2;
    CCSprite* bg_castle;
    
    CCLayer* const parentLayer;
    
private:
    void func_mountainMove();
    void func_cloudMove();
    
private:
    SGBackground(CCLayer* parent);
    static SGBackground* sharedSGBackground;
    
public:
    virtual ~SGBackground();
    static SGBackground* sharedInstance(CCLayer *parent);
};

#endif /* defined(__SketchGame_TouchInput__SGBackground__) */
>>>>>>> bg modifying..
