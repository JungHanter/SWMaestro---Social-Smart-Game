#pragma once
#include "cocos2d.h"
#include "defines.h"
#include "SGSaveData.h"

using namespace cocos2d;

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
    
private:
    CCLabelAtlas* statusLabel[4];
    CCLabelAtlas* statusAddLabel[4];
    CCLabelAtlas* inkLabel;
    
    SGSaveData* saveData;    
    
public:
	SGUpgradeScene(void) {};
	virtual ~SGUpgradeScene(void);

	//static SGHeroInfo getUpgradeInfo();
	static cocos2d::CCScene *scene();
	bool init();
	LAYER_CREATE_FUNC(SGUpgradeScene);
};
