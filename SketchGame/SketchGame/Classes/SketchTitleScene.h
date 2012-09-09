#pragma once

#include "cocos2d.h"
#include "defines.h"
using namespace cocos2d;

#include "SGBackground.h"

#define MAX_ITEM 5

class SketchTitleLayer : public cocos2d::CCLayerColor
{
public:
	SketchTitleLayer(){}
	virtual ~SketchTitleLayer(){ if(background!=NULL) background->release(); };
    bool init();
	void update(float);

    LAYER_CREATE_FUNC(SketchTitleLayer);

private:

	void btnStart(CCObject *sender);
	void btnUpgrade(CCObject *sender);
	void btnRanking(CCObject *sender);
	void btnOption(CCObject *sender);
	void btnExit(CCObject *sender);

private:
	SGBackground* background;

	CCSprite *titleSprite;
	CCSprite *charSprite;
	CCSprite *ropeSprite;

	CCMenuItemSprite *spriteItem[MAX_ITEM+1];
	CCMenu *menu;

	CCActionInterval* act_char;
};

class SketchTitleScene : public cocos2d::CCScene
{
public :
	SketchTitleScene() : _layer(NULL) {}
	~SketchTitleScene(){};
    bool init();

	SCENE_CREATE_FUNC(SketchTitleScene);
    CC_SYNTHESIZE_READONLY(SketchTitleLayer*, _layer, Layer);
};