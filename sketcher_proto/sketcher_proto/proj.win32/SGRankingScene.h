#pragma once

#include "cocos2d.h"

using namespace cocos2d;


class SketchRankingLayer : public cocos2d::CCLayerColor
{
public:
	SketchRankingLayer(){}
	virtual ~SketchRankingLayer(){};
    bool init();
	void update(float);

    LAYER_CREATE_FUNC(SketchRankingLayer);

	int state;

private:
	void btnBack(CCObject *obj);

private:

	bool showing;

	CCSprite *bgi_;
	CCSprite *bgi;
	CCMenu *menu;
	CCMenuItemImage *backItem;
	
};


class SketchRankingScene : public cocos2d::CCScene
{
public :
	SketchRankingScene() : _layer(NULL) {}
	~SketchRankingScene(){};
    bool init();

	SCENE_CREATE_FUNC(SketchRankingScene);
    CC_SYNTHESIZE_READONLY(SketchRankingLayer*, _layer, Layer);


};