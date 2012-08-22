#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CLayerAboutMenu : public CCLayer
{
public:
	CLayerAboutMenu(void);
	~CLayerAboutMenu(void);

	void setOpacity(unsigned char opa);

	void leave();
private:
	void update(float dt);

	void cbBack(CCObject *sender);

	CCMenuItemImage *pBackItem;
	CCMenu *pMenu;

	int animation;
	int state;
};

