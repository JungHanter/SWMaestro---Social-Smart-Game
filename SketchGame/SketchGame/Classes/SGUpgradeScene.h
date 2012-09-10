#pragma once
#include "cocos2d.h"
#include "defines.h"
#include "SGHero.h"
using namespace cocos2d;
extern SGHeroInfo info_upgrade;
class SGUpgradeScene : public cocos2d::CCLayer
{
private:
	CCMenuItemImage* str_image;
	CCMenuItemImage* dex_image;
	CCMenuItemImage* con_image;
	CCMenuItemImage* luck_image;
	CCMenuItemImage* exit_image;

	void strUp(CCObject* pSender);
	void dexUp(CCObject* pSender);
	void conUp(CCObject* pSender);
	void luckUp(CCObject* pSender);
	void exit_Upgrade(CCObject* pSender);
public:
	SGUpgradeScene(void);
	~SGUpgradeScene(void);

	//static SGHeroInfo getUpgradeInfo();
	static cocos2d::CCScene *scene();
	bool init();
	 LAYER_CREATE_FUNC(SGUpgradeScene);
};
