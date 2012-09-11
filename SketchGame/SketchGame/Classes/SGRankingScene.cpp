//
//  SGRankingScene.cpp
//  SketchGame
//
//  Created by Jung Hanter on 12. 9. 12..
//
//

#include "SGRankingScene.h"
#include "SketchTitleScene.h"
#include "SGRankingScene.h"

void SGRankingLayer::back()
{
    CCDirector::sharedDirector()->replaceScene(SketchTitleScene::create());
}

bool SGRankingLayer::init()
{
    if(CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        SGSaveData* saveData = SGSaveData::sharedSaveData();
        const unsigned long long *scroes = saveData->getHeroInfo().score;
        
        int padding = 20;
        int yPos = winSize.height/6;
        int xPos1 = winSize.width/2 - padding;
        int xPos2 = winSize.width - padding;
        
        for(int i=0; i<5; i++) {
            scoreLabel[i] = CCLabelAtlas::create(CCString::createWithFormat("%ld", scroes[i])->getCString(),
                                                 "numbers.png", 20, 38, '0');
            scoreLabel[i]->retain();
            scoreLabel[i]->setAnchorPoint(ccp(1, 0.5));
            scoreLabel[i]->setPosition(xPos1, winSize.height-yPos*(i+1));
            this->addChild(scoreLabel[i], 1);
        }
        for(int i=5; i<10; i++) {
            scoreLabel[i] = CCLabelAtlas::create(CCString::createWithFormat("%ld", scroes[i])->getCString(),
                                                 "numbers.png", 20, 38, '0');
            scoreLabel[i]->retain();
            scoreLabel[i]->setAnchorPoint(ccp(1, 0.5));
            scoreLabel[i]->setPosition(xPos2, winSize.height-yPos*(i-5+1));
            this->addChild(scoreLabel[i], 1);
        }
        
        CCMenuItemImage* exit_image = CCMenuItemImage::create("back.png","back_p.png",this,menu_selector(SGRankingLayer::back));
        exit_image->setScale(0.5f);
        CCMenu* pexit = CCMenu::create(exit_image, NULL);
        pexit->setAnchorPoint(ccp(1,1));
        pexit->setPosition(ccp(winSize.width-30,winSize.height-15));
        
        this->addChild(pexit, 2);
        
        return true;
    } else {
        return false;
    }
}


CCScene* SGRankingLayer::scene()
{
	CCScene * scene = NULL;
	do
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		// 'layer' is an autorelease
		cocos2d::CCLayer *layer = SGRankingLayer::create();
		CC_BREAK_IF(! layer);
        
		// add layer as a child to scene
		scene->addChild(layer);
        
	} while (0);
    
	// return the scene
	return scene;
}

SGRankingLayer::~SGRankingLayer(void) {
    for(int i=0; i<10; i++)
        scoreLabel[i]->release();
}