//
//  SketchGameScene.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#include "SketchGameScene.h"
#include "SketchTitleScene.h"

using namespace cocos2d;

////////////////////////////SktechGameLayer class////////////////////////

////////public method////////
void SketchGameLayer::tapInputEnded(const CCPoint& point) {
    if(CCRect::CCRectContainsPoint(obj_stone->boundingBox(), point)) {
        //CCLog("tapped Stone");
        if(gameState == GAMESTATE_RUNNING &&
           obj_stone->getPosition().y > HERO_HIDE_ABLE_POS.y) {
            //CCLog("hero hide");
            nowObject = OBJECT_STONE;
            hideHeroObject();
        }
    } else if(CCRect::CCRectContainsPoint(obj_grass->boundingBox(), point)) {
        //CCLog("tapped Stone");
        if(gameState == GAMESTATE_RUNNING &&
           obj_grass->getPosition().y > HERO_HIDE_ABLE_POS.y) {
            //CCLog("hero hide");
            nowObject = OBJECT_GRASS;
            hideHeroObject();
        }
    }
}

void SketchGameLayer::dragInputEnded(const int dragDirection) {
    if((gameState==GAMESTATE_BATTLE && turn == TURN_MONSTER) && !bDragSuccess) {
        
        SGAttackInfo nowInfo = nowMonster->getNowAttackInfo();
        
        CCLog("drag dir = %d, monsterdir = %d", dragDirection, nowInfo.atkDir);
        
        switch (nowMonster->getNowAttackState()) {
            case MONSTER_ATK_FIRST:
                if(nowInfo.atkDir == dragDirection) {
                    CCLog("Hero dodge");
                    bDragSuccess = true;
                    defendHero(nowInfo, DEF_STATE_DODGE);
                }
                break;
                
            case MONSTER_ATK_SECOND:
                if(nowInfo.atkDir == dragDirection) {
                    CCLog("Heor guard");
                    bDragSuccess = true;
                    defendHero(nowInfo, DEF_STATE_GUARD);
                }
                break;
        }
        
    }
}


void SketchGameLayer::beginBattleMode() {
    nowMonster->beginBattle();
    gameState = GAMESTATE_BATTLE;
    pauseAllBackground();
    hero->pauseSchedulerAndActions();
    hero->func_wating();
	labelTurn->setString("BEGIN BATTLE");
    CCLog("Begin battle");
	turn = TURN_HERO;
	
    if(nowMonster->isWakeupMonster()) {
        int n = nowMonster->func_wakeup();
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_wakeupAfterAttack), GAME_FRAME_SPEED*(n+2));
    } else {
        nowMonster->func_waiting();
        attackHero();
    }
	//this->scheduleOnce(schedule_selector(SketchGameLayer::endBattleMode), GAME_FRAME_SPEED*5);
}

void SketchGameLayer::func_wakeupAfterAttack(float) {
    attackHero();
}

void SketchGameLayer::monsterAttack(float )
{
	if(turn == TURN_MONSTER)
	{
        bDragSuccess = false;
		SGAttackInfo info = nowMonster->attack();
		
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_watingHeroDefendInput), GAME_FRAME_SPEED*info.nFrames);
	}
}

void SketchGameLayer::func_watingHeroDefendInput(float) {
    if(!bDragSuccess) {
        CCLog("hero defend");
        defendHero(nowMonster->getNowAttackInfo(), DEF_STATE_DEFEND);
    }
}

void SketchGameLayer::defendHero(SGAttackInfo info, int defState) {
    if (hero->defend(info.atk, defState)) {
        this->scheduleOnce(schedule_selector(SketchGameLayer::update_hp), GAME_FRAME_SPEED*info.nFrames);
        if(defState==DEF_STATE_DODGE)
            this->scheduleOnce(schedule_selector(SketchGameLayer::turnHero),4.0f);
        else
            this->scheduleOnce(schedule_selector(SketchGameLayer::turnHero),2.0f);
    } else {
        //hp_bar->runAction(hp_bar_gage[0]);
        this->scheduleOnce(schedule_selector(SketchGameLayer::gameOver), 5);
    }
}

void SketchGameLayer::turnHero(float)
{
	turn = TURN_HERO;
	labelTurn->setString("TURN : HERO");

	attackHero();
}
void SketchGameLayer::attackHero(){
	nowMonster->defend(hero->attack());
	if(nowMonster->endBattle() == true)
		this->scheduleOnce(schedule_selector(SketchGameLayer::endBattleMode),2.0f);
	else
	{
		labelTurn->setString("TURN : MONSTER");
		turn = TURN_MONSTER;
		hero->defendState = DEF_STATE_DEFEND;
		this->scheduleOnce(schedule_selector(SketchGameLayer::monsterAttack),1.0f);
	}
}

void SketchGameLayer::endBattleMode(float) {
	labelTurn->setString("END BATTLE");
	int nFrames = nowMonster->die();
    
	this->scheduleOnce(schedule_selector(SketchGameLayer::func_startHeroRun),GAME_FRAME_SPEED*nFrames);
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_inkMove),GAME_FRAME_SPEED*nFrames);
    CCLog("End battle");
	//func_startHeroRun();
}


void SketchGameLayer::hideHeroObject() {
    gameState = GAMESTATE_HIDEWAITING;
    
    this->schedule(schedule_selector(SketchGameLayer::func_startHeroHide));
}

void SketchGameLayer::Dodge(){
	hero->func_startHide();
	this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveHide), GAME_FRAME_SPEED*11);
}
void SketchGameLayer::func_startHeroHide(float dt) {
    CCSprite* obj = NULL;
    
    switch (nowObject) {
        case OBJECT_STONE:
            obj = obj_stone;
            break;
            
        case OBJECT_GRASS:
            obj = obj_grass;
            break;
    }
    
    if(obj == NULL) {
        return;
    } else if(obj->getPosition().y <= HERO_HIDE_ABLE_POS.y) {
        this->unschedule(schedule_selector(SketchGameLayer::func_startHeroHide));
        gameState = GAMESTATE_HIDE;
        
        pauseAllBackground();
        hero->func_startHide();
        //hero->runAction(CCSpawn::create(hero_act_hide, CCMoveTo::create(GAME_FRAME_SPEED*9, HERO_HIDE_DEST_POS)));
        
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveHide), GAME_FRAME_SPEED*11);
    }
}

void SketchGameLayer::func_heroMoveHide(float dt) {
    //pauseAllBackground();
    hero->func_MoveHide();
    
    if(gameState != GAMESTATE_BATTLE) {
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*8);
        return;
    }
    
	if(hero->dodgeC == 0)
		this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*8);
	else
		this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*1);

}

void SketchGameLayer::func_heroMoveShow(float dt) {
    hero->func_MoveShow();
    
    if(gameState != GAMESTATE_BATTLE) {
        this->scheduleOnce(schedule_selector(SketchGameLayer::func_startHeroRun), GAME_FRAME_SPEED*11);
        return;
    }
    
	if(hero->dodgeC == 0)
		this->scheduleOnce(schedule_selector(SketchGameLayer::func_startHeroRun), GAME_FRAME_SPEED*11);
	else{
		hero->dodgeC = 0;
		this->scheduleOnce(
				schedule_selector(SketchGameLayer::turnHero),2.0f);
	}

}

void SketchGameLayer::func_startHeroRun(float dt) {
    gameState = GAMESTATE_RUNNING;
    
    resumeAllBackground();
    hero->resumeSchedulerAndActions();
    hero->func_startRun();
}

void SketchGameLayer::func_inkMove(float) {
    ink_move->runAction(ink_move_act);
}

int SketchGameLayer::getGameState() {
    return gameState;
}

void SketchGameLayer::update_hp(float) {
    int div = hero->getRemainHpDivision();
    if(div<0) div = 0;
    CCLog("hpDivision : %d", div);
    hp_bar->runAction(hp_bar_gage[div]);
}

void SketchGameLayer::update_ink() {
    nowInk += nowMonster->getInkAmount();
    score += nowMonster->getScoreAmount();
    
    ink_bottle->stopAllActions();
    if(nowInk > INK_DIV_4) {
        ink_bottle->runAction(ink_bottle_act[4]);
        scoreAddAmount = 10;
    } else if(nowInk > INK_DIV_3) {
        ink_bottle->runAction(ink_bottle_act[3]);
        scoreAddAmount = 7;
    } else if(nowInk > INK_DIV_2) {
        ink_bottle->runAction(ink_bottle_act[2]);
        scoreAddAmount = 5;
    } else if(nowInk > INK_DIV_1) {
        ink_bottle->runAction(ink_bottle_act[1]);
        scoreAddAmount = 4;
    } else if(nowInk > 0) {
        ink_bottle->runAction(ink_bottle_act[0]);
        scoreAddAmount = 3;
    } else  {
        //
    }
}

void SketchGameLayer::update_score(float) {
    score += scoreAddAmount;
    scoreLabel->setString(CCString::createWithFormat("%d", score)->getCString());
}

void SketchGameLayer::gameOver(float dt) {
    CCDirector::sharedDirector()->replaceScene(SketchTitleScene::create());
    this->release();
}

void SketchGameLayer::pauseGame(CCObject *o) {
    bPlaying = false;
    //CCDirector::sharedDirector()->pause();
    pauseScoreUpdate();
    this->onExit();
    pauseLayer = PauseGameLayer::create(this);
    pauseLayer->retain();
    parent->addChild(pauseLayer, 10);
}

void SketchGameLayer::resumeGame(CCObject *o) {
    bPlaying = true;
    //CCDirector::sharedDirector()->resume();
    this->onEnter();
    if(gameState != GAMESTATE_RUNNING && gameState!= GAMESTATE_HIDEWAITING) {
        pauseAllBackground();
    } else {
        resumeScoreUpdate();
    }
    parent->removeChild(pauseLayer, true);
    pauseLayer->release();
    pauseLayer=NULL;
}

void SketchGameLayer::loadGameTexture() {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    bPlaying = true;
    bDragSuccess = false;
    
    pSpriteFrameCache->addSpriteFramesWithFile("bg_etc.plist", "bg_etc.png");
    pSpriteFrameCache->addSpriteFramesWithFile("game_etc.plist", "game_etc.png");
    pSpriteFrameCache->addSpriteFramesWithFile("hero.plist", "hero.png");
    pSpriteFrameCache->addSpriteFramesWithFile("mud.plist", "mud.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bat.plist", "bat.png");
    pSpriteFrameCache->addSpriteFramesWithFile("wing.plist", "wing.png");
    pSpriteFrameCache->addSpriteFramesWithFile("ball.plist", "ball.png");

    
    //menu
    CCMenuItemSprite* pauseBtn = CCMenuItemSprite::create(CCSprite::create(pSpriteFrameCache->spriteFrameByName("btn_pause.png")),
                                                          CCSprite::create(pSpriteFrameCache->spriteFrameByName("btn_pause_p.png")),
                                                          this,
                                                          SEL_MenuHandler(menu_selector(SketchGameLayer::pauseGame)));
    pauseBtn->setAnchorPoint(ccp(1,1));
    CCArray* pBtns = CCArray::create();
    pBtns->addObject(pauseBtn);
    
    CCMenu* pauseMenu = CCMenu::create(pBtns);
    pauseMenu->setAnchorPoint(ccp(1,1));
    pauseMenu->setPosition(winSize.width-5, winSize.height-5);
    this->addChild(pauseMenu, ORDER_MENU, TAG_TEXTURE);
    pBtns->release();
    
    
    //score
    score = 0;
    scoreAddAmount = 3;
    
    scoreLabel = CCLabelAtlas::create("0", "numbers.png", 20, 38, '0');
    scoreLabel->retain();
    scoreLabel->setAnchorPoint(ccp(1, 1));
    scoreLabel->setPosition(winSize.width-60, winSize.height-5);
    this->addChild(scoreLabel, ORDER_MENU, TAG_TEXTURE);
    
    
    //background
    background = SGBackground::sharedInstance(this);
    background->retain();
    
    const float coefficient = obj_coefficient(23,4);
    
    CCPoint arrObjPoint[23];
    //const float x_stone_add = 225/22, y_stone_add = 250/22;
    for(int i=0; i<23; i++) {
        //225,190 -> 0, -60
        //arrObjPoint[i].setPoint(225 - (x_stone_add*i), 190 - (y_stone_add*i));
        arrObjPoint[i].setPoint((78*(float)pow(23-i, coefficient))-78, (83*(float)pow(23-i, coefficient))-83-49);
        //CCLog("%lf, %lf", arrObjPoint[i].x, arrObjPoint[i].y);
    }
    HERO_HIDE_ABLE_POS = arrObjPoint[15];
    
    //0.3 -> x1.2
    float arrScaling[23];
    for(int i=0; i<23; i++) {
        //arrScaling[i] = 0.3f * (float)pow(i+1, coefficient
        arrScaling[i] = 1.5f - ( 0.3f * (float)pow(23-i, coefficient) );
        //CCLog("i = %lf", arrScaling[i]);
    }
    
    obj_stone = CCSprite::create(pSpriteFrameCache->spriteFrameByName("obj_stone_1.png"));
    obj_stone->retain();
    obj_stone->setScale(arrScaling[0]);
    obj_stone->setPosition(ccp(-500,-500));
    this->addChild(obj_stone, ORDER_OBJECT, TAG_TEXTURE);
    CCArray* pStoneFrames = CCArray::create();
    CCArray* pStoneMoveActions = CCArray::create();
    for(int i=1; i<=4; i++) {
        pStoneFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("obj_stone_%d.png",i)->getCString()));
    }
    
    for(int i=0; i<23; i++) {
        pStoneMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                     CCSpawn::create(CCPlace::create(arrObjPoint[i]), CCScaleTo::create(0, arrScaling[i]))));
    }
    pStoneMoveActions->addObject(CCSpawn::create(CCPlace::create(ccp(-500,-500)), CCScaleTo::create(0, arrScaling[0])));
    
    obj_stone_action = CCSpawn::create(CCRepeat::create(CCAnimate::create(CCAnimation::create(pStoneFrames, GAME_FRAME_SPEED)), 6),         //6x4=24
                                       CCSequence::create(pStoneMoveActions));
    obj_stone_action->retain();
    pStoneFrames->release();
    pStoneMoveActions->release();
    
    obj_grass = CCSprite::create(pSpriteFrameCache->spriteFrameByName("obj_grass_1.png"));
    obj_grass->retain();
    obj_grass->setScale(arrScaling[0]);
    obj_grass->setPosition(ccp(-500,-500));
    this->addChild(obj_grass, ORDER_OBJECT, TAG_TEXTURE);
    CCArray* pGrassFrames = CCArray::create();
    CCArray* pGrassMoveActions = CCArray::create();
    for(int i=1; i<=3; i++) {
        pGrassFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("obj_grass_%d.png",i)->getCString()));
    }
    
    for(int i=0; i<23; i++) {
        pGrassMoveActions->addObject(CCSpawn::create(CCDelayTime::create(GAME_FRAME_SPEED),
                                                     CCSpawn::create(CCPlace::create(arrObjPoint[i]), CCScaleTo::create(0, arrScaling[i]))));
    }
    pGrassMoveActions->addObject(CCSpawn::create(CCPlace::create(ccp(-500,-500)), CCScaleTo::create(0, arrScaling[0])));
    
    obj_grass_action = CCSpawn::create(CCRepeat::create(CCAnimate::create(CCAnimation::create(pGrassFrames, GAME_FRAME_SPEED)), 8),    //8x3=24
                                       CCSequence::create(pGrassMoveActions));
    obj_grass_action->retain();
    pGrassFrames->release();
    pGrassMoveActions->release();
    
    
    //ink
    nowInk = 0;
    
    ink_bottle = CCSprite::create(pSpriteFrameCache->spriteFrameByName("ink_bottle.png"));
    ink_bottle->retain();
    ink_bottle->setAnchorPoint(ccp(0,0));
    ink_bottle->setPosition(ccp(0,0));
    this->addChild(ink_bottle, ORDER_MENU, TAG_TEXTURE);
    
    ink_move = CCSprite::create(pSpriteFrameCache->spriteFrameByName("ink_move_2.png"));
    ink_move->retain();
    ink_move->setAnchorPoint(ccp(0,0));
    ink_move->setPosition(ccp(-500,-500));
    this->addChild(ink_move, ORDER_MENU, TAG_TEXTURE);
    CCArray* pInkMoveFrames = CCArray::create();
    CCArray* pInkMovePos = CCArray::create();
    for(int i=2; i<=9; i++) {
        pInkMoveFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ink_move_%d.png", i)->getCString()));
    }
    pInkMovePos->addObject(CCSequence::create(CCPlace::create(ccp(201, 94)), CCDelayTime::create(GAME_FRAME_SPEED)));
    pInkMovePos->addObject(CCSequence::create(CCPlace::create(ccp(128, 128)), CCDelayTime::create(GAME_FRAME_SPEED)));
    pInkMovePos->addObject(CCSequence::create(CCPlace::create(ccp(69, 132)), CCDelayTime::create(GAME_FRAME_SPEED)));
    pInkMovePos->addObject(CCSequence::create(CCPlace::create(ccp(22, 57)), CCDelayTime::create(GAME_FRAME_SPEED)));
    pInkMovePos->addObject(CCPlace::create(ccp(0,0)));
    ink_move_act = CCSequence::create(CCSpawn::create(CCAnimate::create(CCAnimation::create(pInkMoveFrames, GAME_FRAME_SPEED)),
                                                      CCSequence::create(pInkMovePos)),
                                      CCSpawn::create(CCPlace::create(ccp(-500, -500)), CCCallFunc::create(this, callfunc_selector(SketchGameLayer::update_ink))));
    ink_move_act->retain();
    pInkMovePos->release();
    pInkMoveFrames->release();
    
    for(int i=0; i<5; i++) {
        CCArray* pInkWaveFrames = CCArray::create();
        for(int j=1; j<=4; j++) {
            pInkWaveFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ink_step%d_%d.png", i+1, j)->getCString()));
        }
        ink_bottle_act[i] = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pInkWaveFrames, GAME_FRAME_SPEED)));
        ink_bottle_act[i]->retain();
        pInkWaveFrames->release();
    }
    
    
    //hp
    hp_bar = CCSprite::create(pSpriteFrameCache->spriteFrameByName("hp_10.png"));
    hp_bar->retain();
    hp_bar->setAnchorPoint(ccp(1,0));
    hp_bar->setPosition(ccp(winSize.width, 0));
    this->addChild(hp_bar, ORDER_MENU, TAG_TEXTURE);
    
    for(int i=0; i<11; i++) {
        CCArray* pHpGage = CCArray::create();
        pHpGage->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("hp_%d.png", i)->getCString()));
        hp_bar_gage[i] = CCAnimate::create(CCAnimation::create(pHpGage, GAME_FRAME_SPEED));

        hp_bar_gage[i]->retain();
        pHpGage->release();
    }
    
    hero = SGHero::create(this);
    hero->retain();
    
    //dummy testing heroinfo
    heroInfo.Str=heroInfo.Dex=heroInfo.Con=heroInfo.Luck=10;
    heroInfo.ink=0;
    
    hero->initHeroState(heroInfo);
    

    CCPoint arrMonsterPoint[23];
    for(int i=0; i<23; i++) {
        //260,190 -> 230, -60
        arrMonsterPoint[i].setPoint(220+(10*(float)pow(23-i, coefficient)), (83*(float)pow(23-i, coefficient))-83-49);
        CCLog("%lf, %lf", arrMonsterPoint[i].x, arrMonsterPoint[i].y);
    }
    
    monsters[MONSTER_TYPE_BAT] = SGMonster::create(MONSTER_TYPE_BAT, 20, 2, 4, 47, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_BAT]->retain();
    
    monsters[MONSTER_TYPE_MUD] = SGMonster::create(MONSTER_TYPE_MUD, 30, 1, 5, 31, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_MUD]->retain();
    
    monsters[MONSTER_TYPE_BALL] = SGMonster::create(MONSTER_TYPE_BALL, 40, 4, 8, 100, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_BALL]->retain();
    
    monsters[MONSTER_TYPE_WING] = SGMonster::create(MONSTER_TYPE_WING, 15, 8, 6, 76, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_WING]->retain();
    
    nowMonster = monsters[MONSTER_TYPE_BAT];
    
    background->gameStart();
    this->schedule(schedule_selector(SketchGameLayer::logic_createTarget), GAME_FRAME_SPEED*30.0f);
    this->schedule(schedule_selector(SketchGameLayer::logic_printGameinfo));
    this->schedule(schedule_selector(SketchGameLayer::update_score), GAME_FRAME_SPEED);
}

void SketchGameLayer::pauseScoreUpdate() {
    this->unschedule(schedule_selector(SketchGameLayer::update_score));
}

void SketchGameLayer::resumeScoreUpdate() {
    this->schedule(schedule_selector(SketchGameLayer::update_score), GAME_FRAME_SPEED);
}

void SketchGameLayer::logic_createTarget(float dt) {
    if(gameState != GAMESTATE_RUNNING) return;
    
    func_createObject();
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_createMonster), GAME_FRAME_SPEED*7); //7프레임 뒤
}

void SketchGameLayer::func_createMonster(float dt) {
    if(gameState != GAMESTATE_RUNNING) return;
    
    int rnd = rand()%(MONSTER_TYPE_NUMBER+2);
    //rnd = MONSTER_TYPE_MUD;
    switch (rnd) {
        case MONSTER_TYPE_BAT:
            nowMonster = monsters[MONSTER_TYPE_BAT];
            nowMonster->resetStatus(20, 2, 4, 47);
            nowMonster->appear();
            break;
            
        case MONSTER_TYPE_BALL:
            nowMonster = monsters[MONSTER_TYPE_BALL];
            nowMonster->resetStatus(40, 4, 8, 100);
            nowMonster->appear();
            break;
            
        case MONSTER_TYPE_MUD:
            nowMonster = monsters[MONSTER_TYPE_MUD];
            nowMonster->resetStatus(30, 1, 5, 31);
            nowMonster->appear();
            break;
            
        case MONSTER_TYPE_WING:
            nowMonster = monsters[MONSTER_TYPE_WING];
            nowMonster->resetStatus(15, 8, 6, 76);
            nowMonster->appear();
            break;
            
        default:
            break;
    }
    
    
}

void SketchGameLayer::func_createObject() {
    if(gameState != GAMESTATE_RUNNING) return;
    
    int rnd = rand()%5;
    if(rnd==0) {
        CCLog("Create Stone");
        obj_stone->runAction(obj_stone_action);
    } else if(rnd==1) {
        CCLog("Create Grass");
        obj_grass->runAction(obj_grass_action);
    }else {
        CCLog("Waiting Obj by num %d", rnd);
    }
}

void SketchGameLayer::test() {
    //monster_spider->setColor(ccc3(255,0,0));
    //monster_spider->stopAllActions();
}

void SketchGameLayer::logic_printGameinfo(float dt) {
    /*CCLog("stone position:(%d, %d) size:(%d, %d)", (int)(obj_stone->getPosition().x), (int)(obj_stone->getPosition().y),
          (int)(obj_stone->boundingBox().size.width), (int)(obj_stone->boundingBox().size.width));*/
    /*CCLog("cloud pos : 1(%d, %d) / 2(%d, %d)", (int)(bg_cloud->getPositionX()), (int)(bg_cloud->getPositionY()),
          (int)(bg_cloud2->getPositionX()), (int)(bg_cloud2->getPositionY()));*/
}

void SketchGameLayer::unloadGameTexture() {
    this->removeChildByTag(TAG_TEXTURE, true);
    
    background->release(); background=NULL;
//    delete background;

    obj_stone->release(); obj_stone=NULL;
    obj_stone_action->release(); obj_stone_action=NULL;
    obj_grass->release(); obj_grass=NULL;
    obj_grass_action->release(); obj_grass_action=NULL;
    
    this->removeChild(scoreLabel, true);
    scoreLabel->release(); scoreLabel=NULL;
    
    hero->release(); hero=NULL;
    
    monsters[0]->release(); monsters[0]=NULL;

    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCTextureCache *pTextureCahce = CCTextureCache::sharedTextureCache();
    
    pSpriteFrameCache->removeSpriteFrameByName("game_etc.plist");
    pSpriteFrameCache->removeSpriteFrameByName("hero.plist");
    pSpriteFrameCache->removeSpriteFrameByName("mud.plist");
    pSpriteFrameCache->removeSpriteFrameByName("bat.plist");
    pSpriteFrameCache->removeSpriteFrameByName("wing.plist");
    pSpriteFrameCache->removeSpriteFrameByName("ball.plist");
    pSpriteFrameCache->removeUnusedSpriteFrames();
    
    pTextureCahce->removeTextureForKey("game_etc.png");
    pTextureCahce->removeTextureForKey("hero.png");
    pTextureCahce->removeTextureForKey("mud.png");
    pTextureCahce->removeTextureForKey("bat.png");
    pTextureCahce->removeTextureForKey("wing.png");
    pTextureCahce->removeTextureForKey("ball.png");
    pTextureCahce->removeUnusedTextures();
    
    this->removeChild(pauseLayer, true);

    //CCTextureCache::sharedTextureCache()->removeAllTextures();
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

void SketchGameLayer::pauseAllBackground() {
    background->pauseAllBackground();
    //bg_cloud->pauseSchedulerAndActions();
    //bg_cloud2->pauseSchedulerAndActions();
    obj_stone->pauseSchedulerAndActions();
    obj_grass->pauseSchedulerAndActions();
    
    pauseScoreUpdate();
    //monster_spider->pauseSchedulerAndActions();
}

void SketchGameLayer::resumeAllBackground() {
    background->resumeAllBackground();
    //bg_cloud->resumeSchedulerAndActions();
    //bg_cloud2->resumeSchedulerAndActions();
    obj_stone->resumeSchedulerAndActions();
    obj_grass->resumeSchedulerAndActions();
    
    resumeScoreUpdate();
    //monster_spider->resumeSchedulerAndActions();
}

////////init method////////
bool SketchGameLayer::init() {
    if( CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->_label = CCLabelTTF::create("", "Artial", 32);
        _label->retain();
        //_label->setColor( ccc3(0,0,0) );
        //_label->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(_label);
        
        this->setTouchEnabled(true);
        
        //user codes
        bTouching = false;
        gameState = GAMESTATE_RUNNING;
        nowObject = OBJECT_NONE;

        loadGameTexture();
        
		labelTurn = CCLabelTTF::create("TURN","Arial",35);
		labelTurn->setColor(ccc3(0,0,0));
		labelTurn->setAnchorPoint(ccp(0,1));
		labelTurn->setPosition(ccp(0,320));

		this->addChild(labelTurn,23);

        return true;
    } else {
        return false;
    }
}

SketchGameLayer::~SketchGameLayer() {
    unloadGameTexture();
    
    if (_label)
    {
        _label->release();
        _label = NULL;
    }
}

void SketchGameLayer::setParent(cocos2d::CCScene *_parent) {
    parent = _parent;
}

////////touch method///////
void SketchGameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = true;
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    oldTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    
    return CCLayerColor::ccTouchesBegan(pTouches, pEvent);
}

void SketchGameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
    CCTouch* pTouch = (CCTouch*)(pTouches->anyObject());
    const CCPoint nowTouchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());
    
    if(bTouching) {
        const CCFloat dist = 10.0f;
        if(ccpDistance(oldTouchPoint, nowTouchPoint) < dist) {
            //tap
            tapInputEnded(nowTouchPoint);
        } else {
            //drag
            int dragDirection;
            
            CCPoint aux = ccpNormalize(ccp(nowTouchPoint.x - oldTouchPoint.x,
                                           nowTouchPoint.y - oldTouchPoint.y));
            if(fabsl(aux.x) >= fabsl(aux.y)) {
                if(aux.x>=0) dragDirection = DIR_RIGHT;
                else dragDirection = DIR_LEFT;
            } else {
                if(aux.y>=0) dragDirection = DIR_UP;
                else dragDirection = DIR_DOWN;
            }
            
            dragInputEnded(dragDirection);
        }
    }
    
    bTouching = false;
}

void SketchGameLayer::ccTouchesCancelled(CCSet* pTouches, CCEvent* pEvent) {
    bTouching = false;
}


////////////////////////////SktechGameScene class////////////////////////

bool SketchGameScene::init() {
    if( CCScene::init() )
    {
        this->_layer = SketchGameLayer::create();
        this->_layer->retain();
        this->_layer->setParent(this);
        this->addChild(_layer, 0);
        
        return true;
    } else {
        return false;
    }
}

SketchGameScene::~SketchGameScene() {
    if (_layer)
    {
        _layer->release();
        _layer = NULL;
    }    
}


/////////////////////////PauseGameLayer class//////////////////////////

bool PauseGameLayer::init() {
    if( PauseGameLayer::initWithColor(ccc4(0,0,0,80)) )
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->_label = CCLabelTTF::create("", "Artial", 32);
        _label->retain();
        //_label->setColor( ccc3(0,0,0) );
        //_label->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(_label);
        
        this->setTouchEnabled(true);
        
        CCSpriteFrameCache* pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        
        CCSprite* pauseMenu = CCSprite::create(pSpriteFrameCache->spriteFrameByName("dialog_pause.png"));
        pauseMenu->retain();
        pauseMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(pauseMenu);
        
        CCArray* pMenuItems = CCArray::create();
        CCMenuItemSprite* btn_resume = CCMenuItemSprite::create(CCSprite::create(pSpriteFrameCache->spriteFrameByName("dialog_btn_resume.png")),
                                                                CCSprite::create(pSpriteFrameCache->spriteFrameByName("dialog_btn_resume_p.png")),
                                                                this, SEL_MenuHandler(menu_selector(PauseGameLayer::resumeGame)));
        CCMenuItemSprite* btn_menu = CCMenuItemSprite::create(CCSprite::create(pSpriteFrameCache->spriteFrameByName("dialog_btn_menu.png")),
                                                              CCSprite::create(pSpriteFrameCache->spriteFrameByName("dialog_btn_menu_p.png")),
                                                              this, SEL_MenuHandler(menu_selector(PauseGameLayer::gotoMenu)));
        pMenuItems->addObject(btn_resume);
        pMenuItems->addObject(btn_menu);
        CCMenu* menu = CCMenu::create(pMenuItems);
        menu->setPosition(ccp(winSize.width/2, winSize.height/2-30));
        menu->alignItemsHorizontallyWithPadding(12);
        this->addChild(menu);
        
        return true;
    } else {
        return false;
    }
}

void PauseGameLayer::resumeGame(CCObject *o) {
    gameLayer->resumeGame(o);
}

void PauseGameLayer::gotoMenu(CCObject *o) {
    gameLayer->gameOver(0);
}

PauseGameLayer* PauseGameLayer::create(SketchGameLayer* _gameLayer) {
    PauseGameLayer* pauseLayer = create();
    pauseLayer->setGameLayer(_gameLayer);
    return pauseLayer;
}

void PauseGameLayer::setGameLayer(SketchGameLayer* _gameLayer) {
    gameLayer = _gameLayer;
}
