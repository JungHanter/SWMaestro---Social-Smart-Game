//
//  SketchGameScene.cpp
//  SketchGame_TouchInput
//
//  Created by Jung Hanter on 12. 8. 18..
//
//

#include "SketchGameScene.h"

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
	switch (dragDirection) {
		case DIR_UP:
			if(gameState == GAMESTATE_BATTLE){
				if(turn == TURN_HERO)
				{
					
				}
				else if(turn == TURN_MONSTER){
					hero->setDefendAction(DEF_STATE_DODGE,DODGE_UP);
				}
			}
			break;

		case DIR_DOWN:
			if(gameState == GAMESTATE_BATTLE){
				if(turn == TURN_HERO){
				}
				else if(turn == TURN_MONSTER){
					hero->setDefendAction(
						DEF_STATE_DODGE,
						DODGE_DOWN);
				}
			}
			break;
		case DIR_LEFT:
			if(gameState == GAMESTATE_BATTLE){
				if(turn == TURN_HERO){

				}
				else if(turn == TURN_MONSTER){
					hero->setDefendAction(
						DEF_STATE_DODGE,
						DODGE_LEFT);
				}
			}
			break;

        case DIR_RIGHT:
			if(gameState == GAMESTATE_BATTLE){
				if(turn == TURN_HERO){

				}
				else if(turn == TURN_MONSTER){
					hero->setDefendAction(
						DEF_STATE_DODGE,
						DODGE_RIGHT);
				}
			}
            break;
	}
}

////////private method///////
void SketchGameLayer::beginBattleMode() {
    nowMonster->beginBattle();
    gameState = GAMESTATE_BATTLE;
    pauseAllBackground();
    hero->pauseSchedulerAndActions();
	labelTurn->setString("BEGIN BATTLE");
    CCLog("Begin battle");
	turn = TURN_HERO;
	
	attackHero();

	//this->scheduleOnce(schedule_selector(SketchGameLayer::endBattleMode), GAME_FRAME_SPEED*5);
}

void SketchGameLayer::monsterAttack(float )
{
	if(turn == TURN_MONSTER)
	{
		int dmg;
		dmg = nowMonster->attack().atk;	
		hero->defend(dmg);
		if(hero->dodgeC == 1){

			//hero->dodgeC = 0;

			hero->func_startHide();
        
			this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveHide), GAME_FRAME_SPEED*11);
		}
		else{
			this->scheduleOnce(
				schedule_selector(SketchGameLayer::turnHero),2.0f);
		}
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
	nowMonster->die();
	nowMonster->endBattle();
	nowMonster->resetStatus(20,2);
	gameState = GAMESTATE_RUNNING;
    resumeAllBackground();
    hero->resumeSchedulerAndActions();
	this->scheduleOnce(schedule_selector(SketchGameLayer::func_startHeroRun),0.0f);
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
    
	if(hero->dodgeC == 0)
		this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*8);
	else
		this->scheduleOnce(schedule_selector(SketchGameLayer::func_heroMoveShow), GAME_FRAME_SPEED*1);

}

void SketchGameLayer::func_heroMoveShow(float dt) {
    hero->func_MoveShow();
    
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
    hero->func_startRun();
}

int SketchGameLayer::getGameState() {
    return gameState;
}

void SketchGameLayer::loadGameTexture() {
    CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    pSpriteFrameCache->addSpriteFramesWithFile("bg_map_hill.plist", "bg_map_hill.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_map_hill2.plist", "bg_map_hill2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave1.plist", "bg_cave1.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave2.plist", "bg_cave2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_into1.plist", "bg_cave_into1.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_into2.plist", "bg_cave_into2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_into3.plist", "bg_cave_into3.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_out.plist", "bg_cave_out.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_cave_out2.plist", "bg_cave_out2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest.plist", "bg_forest.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest_into.plist", "bg_forest_into.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest_into2.plist", "bg_forest_into2.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_forest_additional.plist", "bg_forest_additional.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bg_etc.plist", "bg_etc.png");
    pSpriteFrameCache->addSpriteFramesWithFile("game_etc.plist", "game_etc.png");
    pSpriteFrameCache->addSpriteFramesWithFile("hero.plist", "hero.png");
    pSpriteFrameCache->addSpriteFramesWithFile("mud.plist", "mud.png");
    pSpriteFrameCache->addSpriteFramesWithFile("bat.plist", "bat.png");
    pSpriteFrameCache->addSpriteFramesWithFile("wing.plist", "wing.png");
    pSpriteFrameCache->addSpriteFramesWithFile("ball.plist", "ball.png");

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
    
    
    hero = SGHero::sharedInstance(this);
    hero->retain();
    
    //dummy testing heroinfo
    SGHeroInfo info;
    info.Str=info.Dex=info.Con=info.Luck=10;
    
    hero->initHeroState(info);
    

    CCPoint arrMonsterPoint[23];
    for(int i=0; i<23; i++) {
        //260,190 -> 230, -60
        arrMonsterPoint[i].setPoint(220+(10*(float)pow(23-i, coefficient)), (83*(float)pow(23-i, coefficient))-83-49);
        CCLog("%lf, %lf", arrMonsterPoint[i].x, arrMonsterPoint[i].y);
    }
    
    monsters[MONSTER_TYPE_BAT] = SGMonster::create(MONSTER_TYPE_BAT, 20, 2, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_BAT]->retain();
    
    monsters[MONSTER_TYPE_MUD] = SGMonster::create(MONSTER_TYPE_MUD, 20, 1, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_MUD]->retain();
    
    monsters[MONSTER_TYPE_BALL] = SGMonster::create(MONSTER_TYPE_BALL, 40, 4, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_BALL]->retain();
    
    monsters[MONSTER_TYPE_WING] = SGMonster::create(MONSTER_TYPE_WING, 15, 8, arrMonsterPoint, 23, this);
    monsters[MONSTER_TYPE_WING]->retain();
    
    nowMonster = monsters[MONSTER_TYPE_BAT];
    
    this->schedule(schedule_selector(SketchGameLayer::logic_createTarget), GAME_FRAME_SPEED*30.0f);
    
    this->schedule(schedule_selector(SketchGameLayer::logic_printGameinfo));
}

void SketchGameLayer::logic_createTarget(float dt) {
    if(gameState != GAMESTATE_RUNNING) return;
    
    func_createObject();
    this->scheduleOnce(schedule_selector(SketchGameLayer::func_createMonster), GAME_FRAME_SPEED*7); //7프레임 뒤
}

void SketchGameLayer::func_createMonster(float dt) {
    if(gameState != GAMESTATE_RUNNING) return;
    
    int rnd = rand()%MONSTER_TYPE_NUMBER;
    switch (rnd) {
        case MONSTER_TYPE_BAT:
            nowMonster = monsters[MONSTER_TYPE_BAT];
            nowMonster->resetStatus(20, 2);
            break;
            
        case MONSTER_TYPE_BALL:
            nowMonster = monsters[MONSTER_TYPE_BALL];
            nowMonster->resetStatus(40, 4);
            break;
            
        case MONSTER_TYPE_MUD:
            nowMonster = monsters[MONSTER_TYPE_MUD];
            nowMonster->resetStatus(20, 1);
            break;
            
        case MONSTER_TYPE_WING:
            nowMonster = monsters[MONSTER_TYPE_WING];
            nowMonster->resetStatus(15, 8);
            break;
    }
    
    nowMonster->appear();
}

void SketchGameLayer::func_createObject() {
    if(gameState != GAMESTATE_RUNNING) return;
    
    int rnd = rand()%3;
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
    delete background;

    obj_stone->release(); obj_stone=NULL;
    obj_stone_action->release(); obj_stone_action=NULL;
    obj_grass->release(); obj_grass=NULL;
    obj_grass_action->release(); obj_grass_action=NULL;
    
    hero->release(); hero=NULL;
    delete hero;
    
    monsters[0]->release(); monsters[0]=NULL;

    CCSpriteFrameCache *pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCTextureCache *pTextureCahce = CCTextureCache::sharedTextureCache();
    
    pFrameCache->removeSpriteFramesFromFile("backgroud_forest.plist");
    pTextureCahce->removeTextureForKey("background_forest.png");
    
    //CCTextureCache::sharedTextureCache()->removeAllTextures();
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

void SketchGameLayer::pauseAllBackground() {
    background->pauseAllBackground();
    //bg_cloud->pauseSchedulerAndActions();
    //bg_cloud2->pauseSchedulerAndActions();
    obj_stone->pauseSchedulerAndActions();
    obj_grass->pauseSchedulerAndActions();
    
    //monster_spider->pauseSchedulerAndActions();
}

void SketchGameLayer::resumeAllBackground() {
    background->resumeAllBackground();
    //bg_cloud->resumeSchedulerAndActions();
    //bg_cloud2->resumeSchedulerAndActions();
    obj_stone->resumeSchedulerAndActions();
    obj_grass->resumeSchedulerAndActions();
    
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
        //GameOverLayer::node() is deprecated
        this->_layer = SketchGameLayer::create();
        this->_layer->retain();
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

