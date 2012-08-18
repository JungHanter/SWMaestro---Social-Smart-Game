//
//  SketchGameScene.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#ifndef __SketchGame_TouchInput__SketchGameScene__
#define __SketchGame_TouchInput__SketchGameScene__

#include "cocos2d.h"

class SketchGameLayer : public cocos2d::CCLayerColor
{
public:
    
private:
    void loadGameTexture();
    void unloadGameTexture();
    
private:
    cocos2d::CCSprite* bg_map;
    cocos2d::CCAction* bg_map_action;
    
public:
    SketchGameLayer() : _label(NULL) {}
    ~SketchGameLayer();
    bool init();
    
    LAYER_CREATE_FUNC(SketchGameLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
};

class SketchGameScene : public cocos2d::CCScene
{
public:
    SketchGameScene() : _layer(NULL) {}
    ~SketchGameScene();
    bool init();
    
    SCENE_CREATE_FUNC(SketchGameScene);
    CC_SYNTHESIZE_READONLY(SketchGameLayer*, _layer, Layer);
};


#endif /* defined(__SketchGame_TouchInput__SketchGameScene__) */
