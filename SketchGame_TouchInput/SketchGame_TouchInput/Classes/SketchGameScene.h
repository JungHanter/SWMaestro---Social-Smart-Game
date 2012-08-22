//
//  SketchGameScene.h
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#ifndef __SketchGame_TouchInput__SketchGameScene__
#define __SketchGame_TouchInput__SketchGameScene__

#define GAME_FRAME_SPEED 0.15f
#define SPEED_MOUNTAIN 1000
#define SPEED_CLOUD 200

#define HERO_INIT_POS ccp(170,50)
#define HERO_HIDE_ABLE_POS ccp(173, 320-194)
#define HERO_HIDE_DEST_POS ccp(134, 320-241)

#include "cocos2d.h"

class SketchGameLayer : public cocos2d::CCLayerColor
{
public:
    
private:
    //게임 진행(전투, 달리기) 관련 함수
    void hideHeroObject(bool bStone);
    void startBattle();
    
    //게임 진행 관련 셀렉터
    void func_startheroHide();
    void func_heroMoveHide();
    void func_heroMoveShow();
    
    //게임 운영(텍스쳐 로드, 게임로직 등) 관련 함수
    void loadGameTexture();
    void unloadGameTexture();
    void pauseAllBackground();
    void resumeAllBackground();
    
    //게임 운영 관련 셀렉터
    void logic_createObject();
    void logic_printGameinfo();
    void func_cloudMove();
    
private:
    int gameState;
    
    cocos2d::CCSprite* bg_map;
    cocos2d::CCActionInterval* bg_map_action;
    cocos2d::CCSprite* bg_mountain, *bg_mountain2;
    cocos2d::CCSprite* bg_cloud, *bg_cloud2;
    
    cocos2d::CCSprite* obj_stone;
    cocos2d::CCActionInterval* obj_stone_action;
    
    cocos2d::CCSprite* hero;
    cocos2d::CCActionInterval* hero_act_run, *hero_act_hide_stone;

    cocos2d::CCSprite* monster_slime;
    
    enum GameState { GAMESTATE_RUNNING, GAMESTATE_HIDE, GAMESTATE_BATTLE };
    enum TextureOrder { ORDER_FARAWAY_MOUNTAIN=0, ORDER_FARAWAY_CLOUD, ORDER_FARWAY_CASTLE,
        ORDER_BACKGROUND, ORDER_OBJECT, ORDER_ENEMY, ORDER_HERO };
    enum TagInfo { TAG_NORMAL=0, TAG_TEXTURE, TAG_ETC };
    
    
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
