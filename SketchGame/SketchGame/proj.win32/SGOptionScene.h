#pragma once

#include "cocos2d.h"
#include "defines.h"
using namespace cocos2d;


class SGSound;
class SGOptionScene : public cocos2d::CCLayer
{
private:
	SGSound *sound;
	cocos2d::CCSprite *checkSound;
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	void sound_Yes();
	void sound_No();

	cocos2d::CCSprite *exitSprite;
	void exit_option(CCObject* pSender);
public:
	SGOptionScene(void);
	~SGOptionScene(void);

	static cocos2d::CCScene *scene();
	bool init();
	
//	SCENE_CREATE_FUNC(SGOptionScene);
	  LAYER_CREATE_FUNC(SGOptionScene);
};
        