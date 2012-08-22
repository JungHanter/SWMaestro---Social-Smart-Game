#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class CLayerTitleMenu : public cocos2d::CCLayer
{
public:
	CLayerTitleMenu(void);
	~CLayerTitleMenu(void);

	void setOpacity(unsigned char opa);

private:
	void cbNew(CCObject *sender);
	void cbResume(CCObject *sender);
	void cbOption(CCObject *sender);
	void cbRanking(CCObject *sender);

	void leave(int i);
	void update(float dt);

	enum NEXT {
		NEWGAME=1,
		RESUME,
		OPTION,
		ABOUT,
		RANKING
	};

	int animation;
	int next;

	CCMenuItemImage *pNewItem,
					*pResumeItem,
					*pOptionItem,
					*pRankingItem;
	CCMenu			*pMenu;
};

