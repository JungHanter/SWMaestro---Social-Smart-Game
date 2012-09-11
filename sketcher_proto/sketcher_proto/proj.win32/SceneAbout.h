#pragma once

#include "cocos2d.h"

using namespace cocos2d;

#include "LayerAboutBackground.h"
#include "LayerAboutMenu.h"

class CSceneAbout : public CCLayer
{
public:
	LAYER_CREATE_FUNC(CSceneAbout);

	static CCScene* scene();

	CSceneAbout();
	~CSceneAbout();

	virtual bool init();
	virtual void onEnter();
	
	static CLayerAboutBackground *pLayerBackground;
	static CLayerAboutMenu *pLayerMenu;
};

