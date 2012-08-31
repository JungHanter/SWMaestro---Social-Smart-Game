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
#include "math.h"

#include "defines.h"
#include "SGBackground.h"

class SketchGameLayer : public cocos2d::CCLayerColor
{
public:
    
private:
    //게임 진행(전투, 달리기) 관련 함수
    void hideHeroObject();
    void confirmBattleMode();
    void beginBattleMode();
    void endBattleMode();
    
    //게임 진행 관련 셀렉터
    void func_startHeroHide();
    void func_heroMoveHide();
    void func_heroMoveShow();
    void func_startHeroRun();
    
    //게임 운영(텍스쳐 로드, 게임로직 등) 관련 함수
    void loadGameTexture();
    void unloadGameTexture();
    void pauseAllBackground();
    void resumeAllBackground();
    
    //게임 운영 관련 셀렉터
    void logic_createTarget();
    void func_createMonster();
    void func_createObject();
    void logic_printGameinfo();
    
    void test();
    
private:
    int gameState;
    int nowObject;
    
    cocos2d::CCPoint HERO_HIDE_ABLE_POS;
    
    SGBackground* background;
    
    cocos2d::CCSprite* obj_stone, *obj_grass;
    cocos2d::CCActionInterval* obj_stone_action, *obj_grass_action;
    
    cocos2d::CCSprite* hero;
    cocos2d::CCActionInterval* hero_act_run, *hero_act_hide, *hero_act_show,
        *hero_act_keep;

    cocos2d::CCSprite* monster_spider;
    cocos2d::CCActionInterval* monster_spider_act_run, *monster_spider_act_attack;

    
///////////////touch//////////////
protected:
    void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void ccTouchesCancelled(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
    void dragInputEnded(const int dragDirection);
    void tapInputEnded(const cocos2d::CCPoint& point);
    
private:
    cocos2d::CCPoint oldTouchPoint;
    bool bTouching;
    enum DragDirection { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
    
/////////////basic/////////////
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
