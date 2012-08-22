#pragma once

#include "cocos2d.h"

using namespace cocos2d;

#include "LayerTitleMenu.h"
#include "LayerTitleBackground.h"

#include <vector>

class CSceneTitle : public CCLayer
{
public:
	LAYER_CREATE_FUNC(CSceneTitle);

	static CCScene* scene();

	virtual bool init();
	virtual void onEnter();
	virtual void draw();

	void erase();
	void cbErase(float dt);

	static CLayerTitleMenu *pLayerMenu;
	static CLayerTitleBackground *pLayerBackground;

private:

	bool erasing;
	CCSprite *sprite,*sprite2;
	CCRenderTexture *render;

	CCPoint eraserXY;
	std::vector<CCPoint> xy;
	std::vector<int> xyOpa;

	int dir;
};


