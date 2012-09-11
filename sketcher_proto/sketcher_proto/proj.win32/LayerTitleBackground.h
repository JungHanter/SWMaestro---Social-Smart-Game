#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CLayerTitleBackground : public CCLayer
{
public:
	CLayerTitleBackground(void);
	~CLayerTitleBackground(void);

	virtual bool init();
	virtual void onEnter();

	bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	void update(float dt);

private:
	int state;
	int animation;

	CCLabelTTF *pTouch;

	CCSprite* pBgiColor;
	CCSprite* pBgiWb;
};
