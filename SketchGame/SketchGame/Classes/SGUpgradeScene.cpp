#include "SGUpgradeScene.h"
#include "SketchTitleScene.h"
//SGHeroInfo SGUpgradeScene::info;
SGHeroInfo info_upgrade;
SGUpgradeScene::SGUpgradeScene(void)
{
}

SGUpgradeScene::~SGUpgradeScene(void)
{
}

CCScene* SGUpgradeScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		// 'layer' is an autorelease 
		cocos2d::CCLayer *layer = SGUpgradeScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}
bool SGUpgradeScene::init()
{
	str_image = CCMenuItemImage::create("str.png","str_p.png",this,menu_selector(SGUpgradeScene::strUp));
	CCMenu* pstr = CCMenu::create(str_image, NULL);
	pstr->setPosition(ccp(100,150));
	this->addChild(pstr, 1);

	dex_image = CCMenuItemImage::create("dex.png","dex_p.png",this,menu_selector(SGUpgradeScene::dexUp));
	CCMenu* pdex = CCMenu::create(dex_image, NULL);
	pdex->setPosition(ccp(200,150));
	this->addChild(pdex, 1);


	con_image = CCMenuItemImage::create("con_1.png","con_p.png",this,menu_selector(SGUpgradeScene::conUp));
	CCMenu* pcon = CCMenu::create(con_image, NULL);
	pcon->setPosition(ccp(300,150));
	this->addChild(pcon, 1);


	luck_image = CCMenuItemImage::create("luck.png","luck_p.png",this,menu_selector(SGUpgradeScene::luckUp));
	CCMenu* pluck = CCMenu::create(luck_image, NULL);
	pluck->setPosition(ccp(400,150));
	this->addChild(pluck, 1);

	exit_image = CCMenuItemImage::create("CloseNormal.png","CloseSelected.png",this,menu_selector(SGUpgradeScene::exit_Upgrade));
	CCMenu* pexit = CCMenu::create(exit_image, NULL);
	pexit->setPosition(ccp(200,300));
	this->addChild(pexit, 1);


	return true;
}

void SGUpgradeScene::strUp( CCObject* pSender )
{
	info_upgrade.Str +=1;
}

void SGUpgradeScene::dexUp( CCObject* pSender )
{
	info_upgrade.Dex +=1;
}

void SGUpgradeScene::conUp( CCObject* pSender )
{
	info_upgrade.Con +=1;
}

void SGUpgradeScene::luckUp( CCObject* pSender )
{
	info_upgrade.Luck +=1;
}

void SGUpgradeScene::exit_Upgrade( CCObject* pSender )
{
	CCScene *scene = SketchTitleScene::create();
	CCDirector::sharedDirector()->replaceScene(scene);
}

//SGHeroInfo SGUpgradeScene::getUpgradeInfo()
//{
//	return info;
//}
