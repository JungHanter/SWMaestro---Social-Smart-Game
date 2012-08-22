#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CLayerGameoverMenu : public CCLayer
{
public:
	CLayerGameoverMenu(void);
	~CLayerGameoverMenu(void);

private:
	void cbRetry(CCObject *sender);
	void cbBack(CCObject *sender);

	void update(float dt);

	CCMenuItemImage *pRetryItem,
					*pBackItem;
	CCMenu *pMenu;

	int animation;
};

