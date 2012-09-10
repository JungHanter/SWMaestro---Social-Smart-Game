#include "SGOptionScene.h"
#include "SketchTitleScene.h"

bool onSound2=true;

SGOptionScene::SGOptionScene(void)
{
}

SGOptionScene::~SGOptionScene(void)
{
}

CCScene* SGOptionScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		// 'layer' is an autorelease 
		cocos2d::CCLayer *layer = SGOptionScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}
bool SGOptionScene::init()
{
	this->setTouchEnabled(true);
	
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* pSprite = CCSprite::create("option.png");
	 pSprite->setPosition(ccp(size.width/2, size.height/2));
	this->addChild(pSprite,0);

	checkSound = CCSprite::create("option_check.png",CCRectMake(0,0,64,64));
	checkSound->setAnchorPoint(ccp(0,1));
	 checkSound->setPosition(ccp(230,180));
	this->addChild(checkSound,0);
//	onSound = true;

	//
	CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pSpriteFrameCache->addSpriteFramesWithFile("title_menu.plist", "title_menu.png");

	CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(CCSprite::create(pSpriteFrameCache->spriteFrameByName("menu_exit.png")),CCSprite::create(pSpriteFrameCache->spriteFrameByName(
		"menu_exit_p.png")),this,menu_selector(SGOptionScene::exit_option));

	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 100,50));
	this->addChild(pMenu, 1);

	if(onSound2 == true) // 家府掺扁
	{
			sound_Yes();	
		
	}
	else // 家府虐扁
	{
			sound_No();
	
	
	}

	return true;
}


void SGOptionScene::sound_Yes()
{
	sound->resumeAllEffects();
	sound->resumeBackgroundMusic();
	onSound2 = true;
	checkSound->setPosition(ccp(230,180));
}
void SGOptionScene::sound_No()
{
	sound->stopAllEffects();
	sound->stopBackgroundMusic();
	onSound2 = false;
	checkSound->setPosition(ccp(300,160));
}

void SGOptionScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch-> locationInView());

	if(onSound2 == true) // 家府掺扁
	{
		if(location.x > 300.0f && location.x <360.0f &&
			location.y > 100.0f && location.y < 160.0f)
			sound_No();
	}
	else // 家府虐扁
	{
		if(location.x >230.0f && location.x < 290.0f &&
			location.y > 120.0f && location.y < 180.0f )
			sound_Yes();
	}
}

void SGOptionScene::exit_option( CCObject* pSender )
{
	CCScene *scene = SketchTitleScene::create();
	CCDirector::sharedDirector()->replaceScene(scene);
}
