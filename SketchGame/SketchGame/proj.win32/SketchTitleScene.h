#pragma once

#include "cocos2d.h"
#include "defines.h"
using namespace std;

class SketchTitleLayer : public cocos2d::CCLayerColor
{
public:
	SketchTitleLayer() : _label(NULL) {}
    ~SketchTitleLayer();
    bool init();

	 LAYER_CREATE_FUNC(SketchTitleLayer);
	 CC_SYNTHESIZE_READONLY(SketchTitleLayer*, _label, Label);
};

class SketchTitleScene : public cocos2d::CCScene
{
public :
	SketchTitleScene() : _layer(NULL) {}
    ~SketchTitleScene();
    bool init();

	SCENE_CREATE_FUNC(SketchTitleScene);
    CC_SYNTHESIZE_READONLY(SketchTitleScene*, _layer, Layer);
};