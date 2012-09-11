#include "SGRankingScene.h"

bool SketchRankingLayer::init(){
if( CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
		
		bgi_ = CCSprite::create("ranking_background_.png");
		bgi_->setPosition(
					ccp(240,160)
					);
		this->addChild(bgi_);
		bgi = CCSprite::create("ranking_background.png");
		bgi->setPosition(
					ccp(240,560)
					);
		this->addChild(bgi);


		backItem = CCMenuItemImage::create(
								"back.png",
								"back_p.png",
								this,
								menu_selector(
									SketchRankingLayer::btnBack)
								);
		backItem->setPosition(
						ccp(386,465)
						);

		menu = CCMenu::create(
						backItem,
						NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu);

		this->scheduleUpdate();

		showing = true;

		return true;
	} else {
        return false;
    }


	return true;
}
void SketchRankingLayer::btnBack(CCObject *obj){
	showing = false;
}

void SketchRankingLayer::update(float dt){
	CCLayerColor::update(dt);

	if(showing == true){
		if(backItem->getPositionY() > 65){
			backItem->setPositionY(
				backItem->getPositionY() - 10
				);
		}
		if(bgi->getPositionY() > 160){
			bgi->setPositionY(
				bgi->getPositionY() - 10
				);
		}
	}
	else{
		if(backItem->getPositionY() < 465){
			backItem->setPositionY(
				backItem->getPositionY() + 10
				);
		}
		if(bgi->getPositionY() < 560){
			bgi->setPositionY(
				bgi->getPositionY() + 10
				);
		}
	}
	
}

bool SketchRankingScene::init(){

	this->_layer = SketchRankingLayer::create();
	this->_layer->retain();
    this->addChild(_layer, 0);

	return true;
}

