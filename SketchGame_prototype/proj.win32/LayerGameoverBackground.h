#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CLayerGameoverBackground : public CCLayer
{
public:
	CLayerGameoverBackground(void);
	~CLayerGameoverBackground(void);

private:
	void update(float dt);

	CCSprite *pBgi;
	CCSprite *pGlow;
	CCSprite *pGameover,*pGameover2;
	CCSprite *pEffect;

	int animation;
};

