#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CLayerAboutBackground : public CCLayer
{
public:
	CLayerAboutBackground(void);
	~CLayerAboutBackground(void);

	virtual bool init();
	virtual void onEnter();

private:
	CCSprite *pBgi;
};

