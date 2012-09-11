#include "LayerTitleMenu.h"

#include "cspriteanimation.h"

#include "SceneTitle.h"
#include "SceneAbout.h"

CLayerTitleMenu::CLayerTitleMenu(void)
{
	printf("initialize title menu layer\n");

	//setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	pNewItem = CCMenuItemImage::create(
		"title_menu_new_nor.png",
		"title_menu_new_hot.png",
		this,
		menu_selector(CLayerTitleMenu::cbNew));
	pResumeItem = CCMenuItemImage::create(
		"title_menu_resume_nor.png",
		"title_menu_resume_hot.png",
		this,
		menu_selector(CLayerTitleMenu::cbResume));
	pOptionItem = CCMenuItemImage::create(
		"title_menu_option_nor.png",
		"title_menu_option_hot.png",
		this,
		menu_selector(CLayerTitleMenu::cbOption));
	pRankingItem = CCMenuItemImage::create(
		"title_menu_ranking_nor.png",
		"title_menu_ranking_hot.png",
		this,
		menu_selector(CLayerTitleMenu::cbRanking));

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
	pMenu->setOpacity(0);

	this->addChild(pMenu);
}

CLayerTitleMenu::~CLayerTitleMenu(void)
{
}

void CLayerTitleMenu::leave(int next){
	animation = 255;
	this->next = next;

	this->schedule(schedule_selector(CLayerTitleMenu::update),0.01f);
}

void CLayerTitleMenu::update(float dt){
	animation -= 5;
	pMenu->setOpacity(animation);
	if(animation == 0){
		printf("transition ok\n");
		CCScene *scene;

		switch(next){
		case NEWGAME:
			break;
		case RESUME:
			break;
		case OPTION:
			scene = CSceneAbout::scene();
			CCDirector::sharedDirector()->replaceScene(scene);
			break;
		case RANKING:
			break;
		}
	}
}
void CLayerTitleMenu::cbNew(CCObject *sender){
	printf("new game\n");

	leave(NEWGAME);
}
void CLayerTitleMenu::cbResume(CCObject *sender){
	printf("resume\n");

	leave(RESUME);
}
void CLayerTitleMenu::cbOption(CCObject *sender){
	printf("option\n");

	leave(OPTION);
}
void CLayerTitleMenu::cbRanking(CCObject *sender){
	printf("ranking\n");

	CSceneTitle *ct;
	ct = (CSceneTitle*)getParent();
	ct->erase();
}

void CLayerTitleMenu::setOpacity(unsigned char opa){
	pMenu->setOpacity(opa);
}