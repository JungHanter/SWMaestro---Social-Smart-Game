#include "LayerGameoverMenu.h"


CLayerGameoverMenu::CLayerGameoverMenu(void)
{
	printf("initialize gameover menu layer\n");

	pRetryItem = CCMenuItemImage::create(
		"gameover_menu_retry_nor.png",
		"gameover_menu_retry_hot.png",
		this,
		menu_selector(CLayerGameoverMenu::cbRetry));
	pBackItem = CCMenuItemImage::create(
		"gameover_menu_back_nor.png",
		"gameover_menu_back_hot.png",
		this,
		menu_selector(CLayerGameoverMenu::cbBack));

	pRetryItem->setPosition(
					ccp(340,140));
	pBackItem->setPosition(
					ccp(620,140));

	pMenu = CCMenu::create(
						pRetryItem,
						pBackItem,
						NULL);
	pMenu->setPosition(CCPointZero);

	this->addChild(pMenu);

	this->schedule(schedule_selector(CLayerGameoverMenu::update),0.01f);

	animation = 0;
}
CLayerGameoverMenu::~CLayerGameoverMenu(void)
{
}

void CLayerGameoverMenu::cbRetry(CCObject *sender){
	printf("Retry\n");
}
void CLayerGameoverMenu::cbBack(CCObject *sender){
	printf("Back\n");
}

void CLayerGameoverMenu::update(float dt){
	animation += 1;
	pMenu->setOpacity(animation);
	if(animation == 255)
		this->unschedule(schedule_selector(CLayerGameoverMenu::update));

}