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
#include "SGMonster.h"
#include "SGHero.h"
#include "AppDelegate.h"

using namespace cocos2d;

#define TURN_HERO  1
#define TURN_MONSTER 2

class PauseGameLayer;

class SketchGameLayer : public cocos2d::CCLayerColor
{
public:
    int getGameState();
    
    void hideHeroObject();
    void beginBattleMode();
    
    void pauseAllBackground();
    void resumeAllBackground();
    
	void Dodge();
    
private:
	void monsterAttack(float );
	void turnHero(float);
    void endBattleMode(float );
	void attackHero();
    void defendHero(SGAttackInfo info, int defState);
    
    void func_startHeroHide(float);
    void func_heroMoveHide(float);
    void func_heroMoveShow(float);
    void func_startHeroRun(float);
    
    void func_watingHeroDefendInput(float);
    void func_wakeupAfterAttack(float);
    void func_inkMove(float);
    
    void loadGameTexture();
    void unloadGameTexture();
    
    void logic_createTarget(float);
    void func_createMonster(float);
    void func_createObject();
    void logic_printGameinfo(float);
    
    void update_hp(float);
    void update_ink();
    void update_score(float);
    
    void pauseScoreUpdate();
    void resumeScoreUpdate();
    
public:
    void gameOver(float dt);
    void pauseGame(CCObject *o);
    void resumeGame(CCObject *o);
    
    void setParent(CCScene* _parent);
    
    void test();
    
private:
	int turn;
    int gameState;
    int nowObject;
    int nowInk;
    int score, scoreAddAmount;
    bool bDragSuccess;
    bool bPlaying;
    bool bPressPauseBtn;
    
    cocos2d::CCPoint HERO_HIDE_ABLE_POS;
    
    CCLabelAtlas* scoreLabel;
    CCScene* parent;
    PauseGameLayer* pauseLayer;
    
    SGBackground* background;
    
    SGMonster* monsters[MONSTER_TYPE_NUMBER];
    SGMonster* nowMonster;
    
    cocos2d::CCSprite* obj_stone, *obj_grass;
    cocos2d::CCActionInterval* obj_stone_action, *obj_grass_action;
    
    SGHero* hero;
    SGHeroInfo heroInfo;
    
    CCSprite* ink_bottle, *ink_move;
    CCActionInterval* ink_bottle_act[5], *ink_move_act;
    
    CCSprite* hp_bar;
    CCAction* hp_bar_gage[11];
    
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
    enum DragDirection { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
    
    /////////////basic/////////////
public:
    SketchGameLayer() : _label(NULL) {}
    virtual ~SketchGameLayer();
    bool init();
    
    LAYER_CREATE_FUNC(SketchGameLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
};


//////////////////////SketchGameScene//////////////////////
class SketchGameScene : public cocos2d::CCScene
{
public:
    SketchGameScene() : _layer(NULL) {}
    ~SketchGameScene();
    bool init();
    
    SCENE_CREATE_FUNC(SketchGameScene);
    CC_SYNTHESIZE_READONLY(SketchGameLayer*, _layer, Layer);
};

//////////////////////PauseGameLayer////////////////////////
class PauseGameLayer : public CCLayerColor
{
public:
    PauseGameLayer() : _label(NULL) {}
    virtual ~PauseGameLayer() {}
    bool init();
    static PauseGameLayer* create(SketchGameLayer* _gameLayer);
    void setGameLayer(SketchGameLayer* _gameLayer);
    
    LAYER_CREATE_FUNC(PauseGameLayer);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
    
private:
    SketchGameLayer* gameLayer;
    
    void resumeGame(CCObject *o);
    void gotoMenu(CCObject *o);
};


#endif /* defined(__SketchGame_TouchInput__SketchGameScene__) */
