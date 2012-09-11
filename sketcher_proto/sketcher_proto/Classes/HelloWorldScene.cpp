#include "HelloWorldScene.h"

#include "cspriteanimation.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	CCMenuItemImage *pNewItem,
					*pResumeItem,
					*pOptionItem,
					*pRankingItem;
	CCMenu			*pMenu;

	pNewItem = CCMenuItemImage::create(
		"title_menu_new_nor.png",
		"title_menu_new_hot.png",
		this,
		NULL);
	pResumeItem = CCMenuItemImage::create(
		"title_menu_resume_nor.png",
		"title_menu_resume_hot.png",
		this,
		NULL);
	pOptionItem = CCMenuItemImage::create(
		"title_menu_option_nor.png",
		"title_menu_option_hot.png",
		this,
		NULL);
	pRankingItem = CCMenuItemImage::create(
		"title_menu_ranking_nor.png",
		"title_menu_ranking_hot.png",
		this,
		NULL);

	pNewItem->setPosition(ccp(800,270));
	pResumeItem->setPosition(ccp(800,200));
	pOptionItem->setPosition(ccp(800,130));
	pRankingItem->setPosition(ccp(800,60));

	pMenu = CCMenu::create(
						pNewItem,
						pResumeItem,
						pOptionItem,
						pRankingItem,
						NULL);

	pMenu->setPosition(CCPointZero);
	
	
	CCLayer *l = CCLayerColor::layerWithColor(ccc4(255,255,255,255));

	this->addChild(l);
	this->addChild(pMenu);

	return true;
}

