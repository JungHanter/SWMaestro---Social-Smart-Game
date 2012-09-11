#include "LayerAboutMenu.h"

#include "SceneTitle.h"

CLayerAboutMenu::CLayerAboutMenu(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	pBackItem = CCMenuItemImage::create(
		"about_menu_back_nor.png",
		"about_menu_back_hot.png",
		this,
		menu_selector(CLayerAboutMenu::cbBack));
	pBackItem->setPosition(
					ccp(150,50)
					);

	pMenu = CCMenu::create(
						pBackItem,
						NULL);
	pMenu->setPosition(CCPointZero);
	pMenu->setOpacity(0);

	this->addChild(pMenu);

	this->schedule(schedule_selector(CLayerAboutMenu::update),0.01f);

	animation = 0;
	state = 0;
}
CLayerAboutMenu::~CLayerAboutMenu(void)
{
}

void CLayerAboutMenu::cbBack(CCObject *sender){
	leave();
}
void CLayerAboutMenu::leave(){
	animation = 255;
	state = 1;

	this->schedule(schedule_selector(CLayerAboutMenu::update),0.01f);
}
void CLayerAboutMenu::update(float dt){
	if(state == 0){
		if(animation != 255){
			animation += 5;
			pMenu->setOpacity(animation);
		}
		else
			this->unschedule(schedule_selector(CLayerAboutMenu::update));
	}
	else if(state == 1){
		if(animation != 0){
			animation -= 5;
			pMenu->setOpacity(animation);
		}
		else{
			this->unschedule(schedule_selector(CLayerAboutMenu::update));

			CCScene *scene;
			scene = CSceneTitle::scene();
			CCDirector::sharedDirector()->replaceScene(scene);

			printf("go title\n");
		}
	}
}