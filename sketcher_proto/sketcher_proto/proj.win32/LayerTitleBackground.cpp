#include "LayerTitleBackground.h"

#include "SceneTitle.h"

#include "flags.h"

EXTERN_FLAG;

CLayerTitleBackground::CLayerTitleBackground(void)
{
	printf("initialize title background layer\n");

	pBgiColor = CCSprite::create("title_background_color.jpg");
	pBgiWb = CCSprite::create("title_background_wb.jpg");

	pBgiColor->setPosition(
		ccp(480,320)
		);
	pBgiWb->setPosition(
		ccp(480,320)
		);

	pBgiWb->setVisible(false);

	pTouch = CCLabelTTF::create(
									"Touch to Start",
									"Arial",
									45);
	pTouch->setPosition(
		ccp(480,320)
		);
	pTouch->setColor(ccc3(255,0,0));

	this->addChild(pBgiColor,0);
	this->addChild(pBgiWb,0);
	this->addChild(pTouch,0);

	
	state = 0;
	animation = 0;
}
CLayerTitleBackground::~CLayerTitleBackground(void)
{
}

bool CLayerTitleBackground::init(){
	CCLayer::init();
	return true;
}
void CLayerTitleBackground::onEnter(){
	CCLayer::onEnter();

	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	if(flag[string("firstExecute")] != 1){
		state = 3;
		animation = 255;

		pBgiColor->setVisible(false);
		pTouch->setVisible(false);
		pBgiWb->setVisible(true);
		pBgiWb->setOpacity(255);

		this->schedule(schedule_selector(CLayerTitleBackground::update),0.01f);
	}
	else{
		flag[string("firstExecute")] = 0;

		this->schedule(schedule_selector(CLayerTitleBackground::update),0.1f);
	}
}

bool CLayerTitleBackground::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
	CCPoint touchLocation = touch->locationInView();    
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	
	if(state == 0){
		state = 1;
		animation = 0;
	}

	return true;
}
void CLayerTitleBackground::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
}
void CLayerTitleBackground::ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
}
void CLayerTitleBackground::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event){
}

void CLayerTitleBackground::update(float dt){
	printf("update\n");
	if(state == 1){
		animation ++;
		if(animation % 2 == 0){
			pTouch->setColor(ccc3(255,0,0));
		}
		else{
			pTouch->setColor(ccc3(0,255,0));
		}

		if(animation == 4){
			state = 2;
			animation = 255;

			pBgiWb->setVisible(true);
			pBgiWb->setOpacity(0);

			this->schedule(schedule_selector(CLayerTitleBackground::update),0.01f);
		}
	}
	else if(state == 2){
		animation -= 1;

		CSceneTitle::pLayerMenu->setOpacity(255-animation);

		pTouch->setOpacity(animation);

		pBgiColor->setOpacity(animation);
		pBgiWb->setOpacity(255-animation);

		
		if(animation == 0){
			state = 4;
			this->unschedule(schedule_selector(CLayerTitleBackground::update));
		}
	}
	else if(state == 3){
		animation -= 5;

		CSceneTitle::pLayerMenu->setOpacity(255-animation);

		if(animation == 0){
			state = 4;
			this->unschedule(schedule_selector(CLayerTitleBackground::update));
		}
	}
	
}