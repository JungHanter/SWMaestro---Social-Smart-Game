#include "SketchTitleScene.h"
#include "SketchGameScene.h"
#include "SGOptionScene.h"
#include "SGUpgradeScene.h"


bool SketchTitleLayer::init(){
if( CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
		CCSpriteFrameCache *pSpriteFrameCache =
			CCSpriteFrameCache::sharedSpriteFrameCache();

		pSpriteFrameCache->addSpriteFramesWithFile("title_menu.plist", "title_menu.png");;
		pSpriteFrameCache->addSpriteFramesWithFile("opening.plist", "opening.png");;
		pSpriteFrameCache->addSpriteFramesWithFile("opening2.plist", "opening2.png");;
		pSpriteFrameCache->addSpriteFramesWithFile("opening3.plist", "opening3.png");;
		
		background = SGBackground::sharedInstance(this);
		background->retain();
        background->pauseAllBackground();
        background->resetLogic();
        
		titleSprite = CCSprite::create(
			pSpriteFrameCache->spriteFrameByName(
				"title.png"));
        titleSprite->retain();
        titleSprite->setPosition(ccp(-170,210));        
		this->addChild(titleSprite,10);

		ropeSprite = CCSprite::create(
			pSpriteFrameCache->spriteFrameByName(
				"menu_rope.png"));
        ropeSprite->retain();
        ropeSprite->setPosition(ccp(470,-130));        
		this->addChild(ropeSprite,10);


		CCArray* pKissFrames = CCArray::create();
		for(int i=1; i<=7; i++) {
			pKissFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                CCString::createWithFormat("menu_kiss_%d.png",i)->getCString()));
		}
        for(int i=6; i>=2; i--) {
			pKissFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                CCString::createWithFormat("menu_kiss_%d.png",i)->getCString()));
		}
		act_char = CCRepeatForever::create(
			CCAnimate::create(CCAnimation::create(pKissFrames, GAME_FRAME_SPEED)));
		act_char->retain();
		pKissFrames->release();

		charSprite = CCSprite::create(
			pSpriteFrameCache->spriteFrameByName(
				"menu_kiss_1.png"));
		charSprite->setPosition(
						ccp(170,70));
		charSprite->runAction(act_char);
		charSprite->setOpacity(0);
		this->addChild(charSprite,12);


		CCArray* pOpeningFrames = CCArray::create();
		for(int i=1; i<=33; i++) {
			printf("op %d\n", i);
			pOpeningFrames->addObject(pSpriteFrameCache->spriteFrameByName(
                CCString::createWithFormat("opening_%d.png",i)->getCString()));
		}
		
		act_opening = CCSequence::create(
			CCAnimate::create(
				CCAnimation::create(pOpeningFrames, GAME_FRAME_SPEED)),
			CCCallFunc::create(
				this,callfunc_selector(SketchTitleLayer::func_endOpening))
				);
		act_opening->retain();
		pOpeningFrames->release();

		openingSprite = CCSprite::create(
			pSpriteFrameCache->spriteFrameByName(
				"opening_1.png"));
        openingSprite->retain();
		openingSprite->setPosition(
						ccp(140,160));
		openingSprite->setVisible(false);
		this->addChild(openingSprite,20);

		char items[][64] = 
			{"start",
			 "upgrade",
			 "option",
			 "ranking",
			 "exit"};
		SEL_MenuHandler itemsSelector[] = 
			{
			menu_selector(SketchTitleLayer::btnStart),
			menu_selector(SketchTitleLayer::btnUpgrade),
			menu_selector(SketchTitleLayer::btnOption),
			menu_selector(SketchTitleLayer::btnRanking),
			menu_selector(SketchTitleLayer::btnExit)
			};
		CCArray *itemArray = CCArray::create();

		for(int i=0;i<MAX_ITEM;i++){
			char normalSprite[64] = "menu_";
			char selectedSprite[64] = "menu_";

			sprintf(normalSprite, "%s%s.png", normalSprite,items[i]);
			sprintf(selectedSprite, "%s%s_p.png", selectedSprite,items[i]);
            
			spriteItem[i] = CCMenuItemSprite::create(
				CCSprite::create(pSpriteFrameCache->spriteFrameByName(
								normalSprite)),
				CCSprite::create(pSpriteFrameCache->spriteFrameByName(
								selectedSprite)),
								this,
								itemsSelector[i]
								);
			spriteItem[i]->setPosition(
						ccp(380,40+55*(MAX_ITEM-i-1)));

			itemArray->addObject(
						spriteItem[i]);
		}
		spriteItem[MAX_ITEM] = NULL;
		
		menu = CCMenu::create(
						itemArray);
		menu->setPosition(
						ccp(0,-300));

		this->addChild(menu,11);

		state = TITLE;
		opening = false;
		this->scheduleUpdate();

        return true;
        
	} else {
        return false;
    }

}

void SketchTitleLayer::update(float dt){
	CCLayerColor::update(dt);

	if(state == TITLE){
		if(titleSprite->getPositionX() <= 160)
			titleSprite->setPositionX(
				titleSprite->getPositionX() + 5 );

		if(ropeSprite->getPositionY() <= 130)
			ropeSprite->setPositionY(
				ropeSprite->getPositionY() + 5 );

		if(menu->getPositionY() <= 0)
			menu->setPositionY(
				menu->getPositionY() + 5 );

		if(charSprite->getOpacity() < 255)
			charSprite->setOpacity(
					charSprite->getOpacity()+5
				);
	}
	else{
		if(titleSprite->getPositionX() > -160)
			titleSprite->setPositionX(
				titleSprite->getPositionX() - 5 );

		if(ropeSprite->getPositionY() > -130)
			ropeSprite->setPositionY(
				ropeSprite->getPositionY() - 5);

		if(menu->getPositionY() > -300)
			menu->setPositionY(
				menu->getPositionY() - 5 );
		else{
            if(opening == false) {
                openingSprite->runAction(act_opening);
                openingSprite->setVisible(true);
                charSprite->setVisible(false);
                opening = true;
            }
		}

	}
}
void SketchTitleLayer::func_endOpening(){
	CCScene *scene = SketchGameScene::create();
	CCDirector::sharedDirector()->replaceScene(scene);
}
void SketchTitleLayer::btnStart(CCObject *sender){
	if(opening == false){
		
		state = OPENING;
	}
	//CCScene *scene = SketchGameScene::create();
	//CCDirector::sharedDirector()->replaceScene(scene);
	//pDirector->runWithScene(pScene);
}
void SketchTitleLayer::btnUpgrade(CCObject *sender){
	CCScene *scene = SGUpgradeScene::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}
void SketchTitleLayer::btnRanking(CCObject *sender){
}
void SketchTitleLayer::btnOption(CCObject *sender){
	CCScene *scene = SGOptionScene::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}
void SketchTitleLayer::btnExit(CCObject *sender){
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool SketchTitleScene::init(){

	this->_layer = SketchTitleLayer::create();
	this->_layer->retain();
    this->addChild(_layer, 0);

	return true;
}

