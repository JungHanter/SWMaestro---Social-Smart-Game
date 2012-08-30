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
#define SPEED_MOUNTAIN 2000
#define SPEED_CLOUD 500

#define MOUNTAIN_ANCOHR_Y -(425.0f/249.0f)

#define HERO_INIT_POS ccp(200,70)
#define HERO_HIDE_DEST_POS ccp(120,70)
#define HERO_HIDE_MID_POS (ccp((HERO_INIT_POS.x+HERO_HIDE_DEST_POS.x)/2, 70))

#define OBJ_SCALING_F01 0.05f
#define OBJ_SCALING_F02 0.1f
#define OBJ_SCALING_F03 0.15f
#define OBJ_SCALING_F04 0.2f
#define OBJ_SCALING_F05 0.25f
#define OBJ_SCALING_F06 0.3f
#define OBJ_SCALING_F07 0.35f
#define OBJ_SCALING_F08 0.4f
#define OBJ_SCALING_F09 0.45f
#define OBJ_SCALING_F10 0.5f
#define OBJ_SCALING_F11 0.55f
#define OBJ_SCALING_F12 0.6f
#define OBJ_SCALING_F13 0.65f
#define OBJ_SCALING_F14 0.7f
#define OBJ_SCALING_F15 0.75f
#define OBJ_SCALING_F16 0.8f
#define OBJ_SCALING_F17 0.85f
#define OBJ_SCALING_F18 0.9f
#define OBJ_SCALING_F19 0.95f
#define OBJ_SCALING_F20 1.0f
#define OBJ_SCALING_F21 1.05f
#define OBJ_SCALING_F22 1.1f
#define OBJ_SCALING_F23 1.15f


// y = x^a, n등분일때 (1/n * M배) = (n/n)의 a값 구하기
// M * (1/n * x0)^a = (x0)^a
// a = log.n.(M) = log(M)/log(n)
// _N_등분일때 _M_배
#define obj_coefficient(_N_,_M_) (float)(log(_M_)/log(_N_))

// y = x^a
//#define

#include "cocos2d.h"
#include "math.h"

class SketchGameLayer : public cocos2d::CCLayerColor
{
public:
    
private:
    //게임 진행(전투, 달리기) 관련 함수
    void hideHeroObject(bool bStone);
    void startBattle();
    
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
    void logic_createObject();
    void logic_printGameinfo();
    void func_mountainMove();
    void func_cloudMove();
    
private:
    int gameState;
    
    cocos2d::CCPoint HERO_HIDE_ABLE_POS;
    
    cocos2d::CCSprite* bg_map;
    cocos2d::CCActionInterval* bg_map_action;
    cocos2d::CCSprite* bg_mountain, *bg_mountain2;
    cocos2d::CCSprite* bg_cloud, *bg_cloud2;
    cocos2d::CCSprite* bg_castle;
    
    cocos2d::CCSprite* obj_stone, *obj_grass;
    cocos2d::CCActionInterval* obj_stone_action, *obj_grass_action;
    
    cocos2d::CCSprite* hero;
    cocos2d::CCActionInterval* hero_act_run, *hero_act_hide, *hero_act_show,
        *hero_act_keep;

    cocos2d::CCSprite* monster_spider;
    
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
