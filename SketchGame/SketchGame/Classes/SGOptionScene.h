#pragma once

#ifndef __SGOPTION_SCENE_H__
#define __SGOPTION_SCENE_H__

#include "cocos2d.h"
#include "defines.h"
#include "SGSound.h"
using namespace cocos2d;

struct SGOptionInfo {
    bool bSound;
    bool bVibration;
};

class SGSound;
class SGOptionScene : public cocos2d::CCLayer
{
private:
	SGSound *sound;
	cocos2d::CCSprite *checkSound;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    SGSound* pSharedSound;

	void sound_Yes();
	void sound_No();

	cocos2d::CCSprite *exitSprite;
	void exit_option(CCObject* pSender);
public:
	SGOptionScene(void);
	virtual ~SGOptionScene(void);

	static cocos2d::CCScene* scene();
	virtual bool init();
	static bool onSound;
    
    LAYER_CREATE_FUNC(SGOptionScene);
};
        
#endif