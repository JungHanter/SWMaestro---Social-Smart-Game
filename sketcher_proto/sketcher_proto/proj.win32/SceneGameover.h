#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "LayerGameoverBackground.h"
#include "LayerGameoverMenu.h"

class CSceneGameover : public CCLayer
{
public:
	LAYER_CREATE_FUNC(CSceneGameover);

	static CCScene* scene();

	virtual bool init();
	virtual void onEnter();

	static CLayerGameoverMenu *pLayerMenu;
	static CLayerGameoverBackground *pLayerBackground;

};

