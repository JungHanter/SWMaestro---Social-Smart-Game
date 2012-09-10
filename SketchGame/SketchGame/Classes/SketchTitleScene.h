#pragma once

#include "cocos2d.h"
#include "defines.h"
using namespace cocos2d;

#include "SGBackground.h"

#define MAX_ITEM 5

enum SG_TITLE_STATE {
	TITLE,
	OPENING
	};

class SketchTitleLayer : public cocos2d::CCLayerColor
{
public:
	SketchTitleLayer(){}
	virtual ~SketchTitleLayer(){ if(background!=NULL) background->release(); };
    bool init();
	void update(float);

    LAYER_CREATE_FUNC(SketchTitleLayer);

	int state;

private:

	void btnStart(CCObject *sender);
	void btnUpgrade(CCObject *sender);
	void btnRanking(CCObject *sender);
	void btnOption(CCObject *sender);
	void btnExit(CCObject *sender);

	void func_endOpening();

private:
	SGBackground* background;

	CCSprite *titleSprite;
	CCSprite *charSprite;
	CCSprite *ropeSprite;

	CCSprite *openingSprite;
	int opening;

	CCMenuItemSprite *spriteItem[MAX_ITEM+1];
	CCMenu *menu;

	CCActionInterval* act_char;
	CCFiniteTimeAction* act_opening;
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