#include "SGUpgradeScene.h"
#include "SketchTitleScene.h"

#define getStatusAmount(_status_) ( 10 * (_status_-9) )


bool SGUpgradeScene::init()
{
    if(CCLayer::init()) {
        CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        pCache->addSpriteFramesWithFile("game_etc.plist", "game_etc.png");
        
        CCSprite* background = CCSprite::create("upgrade_bgi.png");
        background->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,
                                    CCDirector::sharedDirector()->getWinSize().height/2));
        this->addChild(background, 1);
        
        
        saveData = SGSaveData::sharedSaveData();
        SGHeroInfo heroinfo = saveData->getHeroInfo();
        int status[4] = { heroinfo.Str, heroinfo.Con, heroinfo.Dex, heroinfo.Luck };
        
        
        CCArray* pInkWaveFrames = CCArray::create();
        for(int i=1; i<=4; i++) {
            pInkWaveFrames->addObject(pCache->spriteFrameByName(CCString::createWithFormat("ink_step5_%d.png", i)->getCString()));
        }
        CCSprite* ink_bottle = CCSprite::create(pCache->spriteFrameByName("ink_step5_1.png"));
        ink_bottle->setPosition(ccp(300,255));
        this->addChild(ink_bottle,2);
        ink_bottle->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pInkWaveFrames, GAME_FRAME_SPEED))));
        pInkWaveFrames->release();
        
        for(int i=0; i<4; i++) {
            statusLabel[i] = CCLabelAtlas::create("10", "numbers.png", 20, 38, '0');
            statusLabel[i]->retain();
            statusLabel[i]->setAnchorPoint(ccp(0, 0.5));
            this->addChild(statusLabel[i], 1);
            
            statusAddLabel[i] = CCLabelAtlas::create("10", "numbers_small.png", 10, 19, '0');
            statusAddLabel[i]->retain();
            statusAddLabel[i]->setAnchorPoint(ccp(1, 0.5));
            this->addChild(statusAddLabel[i], 1);
            
            statusLabel[i]->setString(CCString::createWithFormat("%d", status[i])->getCString());
            statusAddLabel[i]->setString(CCString::createWithFormat("%d", getStatusAmount(status[i]) )->getCString());
        }
        inkLabel = CCLabelAtlas::create(CCString::createWithFormat("%ld", heroinfo.ink)->getCString(),
                                        "numbers.png", 20, 38, '0');
        inkLabel->retain();
        inkLabel->setAnchorPoint(ccp(0, 0.5));
        inkLabel->setPosition(ccp(310, 235));
        this->addChild(inkLabel, 1);
         
        str_image = CCMenuItemImage::create("str.png","str_p.png",this,menu_selector(SGUpgradeScene::strUp));
        CCMenu* pstr = CCMenu::create(str_image, NULL);
        pstr->setPosition(ccp(80,180));
        this->addChild(pstr, 1);
        statusLabel[0]->setPosition(ccp(120, 180));
        statusAddLabel[0]->setPosition(ccp(100, 145));
        
        
        con_image = CCMenuItemImage::create("con_1.png","con_p.png",this,menu_selector(SGUpgradeScene::conUp));
        CCMenu* pcon = CCMenu::create(con_image, NULL);
        pcon->setPosition(ccp(80,90));
        this->addChild(pcon, 1);
        statusLabel[1]->setPosition(ccp(120, 90));
        statusAddLabel[1]->setPosition(ccp(100, 55));
            
            
        dex_image = CCMenuItemImage::create("dex.png","dex_p.png",this,menu_selector(SGUpgradeScene::dexUp));
        CCMenu* pdex = CCMenu::create(dex_image, NULL);
        pdex->setPosition(ccp(270,180));
        this->addChild(pdex, 1);
        statusLabel[2]->setPosition(ccp(310, 180));
        statusAddLabel[2]->setPosition(ccp(290, 145));
            
        
        luck_image = CCMenuItemImage::create("luck.png","luck_p.png",this,menu_selector(SGUpgradeScene::luckUp));
        CCMenu* pluck = CCMenu::create(luck_image, NULL);
        pluck->setPosition(ccp(270,90));
        this->addChild(pluck, 1);
        statusLabel[3]->setPosition(ccp(310, 90));
        statusAddLabel[3]->setPosition(ccp(290, 55));
        
        
        exit_image = CCMenuItemImage::create("back.png","back_p.png",this,menu_selector(SGUpgradeScene::exit_Upgrade));
        CCMenu* pexit = CCMenu::create(exit_image, NULL);
        pexit->setPosition(ccp(400,300));
        this->addChild(pexit, 1);
        
        return true;
    }
    
    return false;
}

void SGUpgradeScene::strUp( CCObject* pSender )
{
    SGHeroInfo info = saveData->getHeroInfo();
    int inkAmount = getStatusAmount(info.Str);
    
    if(info.ink >= inkAmount) {
        saveData->changeInk(-inkAmount);
        saveData->addStatusStr();
        
        //refresh
        info = saveData->getHeroInfo();
        inkLabel->setString(CCString::createWithFormat("%ld", info.ink)->getCString());
        statusLabel[0]->setString(CCString::createWithFormat("%d", info.Str)->getCString());
        statusAddLabel[0]->setString(CCString::createWithFormat("%d", getStatusAmount(info.Str))->getCString());
    }
}

void SGUpgradeScene::conUp( CCObject* pSender )
{
    SGHeroInfo info = saveData->getHeroInfo();
    int inkAmount = getStatusAmount(info.Con);
    
    if(info.ink >= inkAmount) {
        saveData->changeInk(-inkAmount);
        saveData->addStatusCon();
        
        //refresh
        info = saveData->getHeroInfo();
        inkLabel->setString(CCString::createWithFormat("%ld", info.ink)->getCString());
        statusLabel[1]->setString(CCString::createWithFormat("%d", info.Con)->getCString());
        statusAddLabel[1]->setString(CCString::createWithFormat("%d", getStatusAmount(info.Con))->getCString());
    }
}


void SGUpgradeScene::dexUp( CCObject* pSender )
{
    SGHeroInfo info = saveData->getHeroInfo();
    int inkAmount = getStatusAmount(info.Dex);
    
    if(info.ink >= inkAmount) {
        saveData->changeInk(-inkAmount);
        saveData->addStatusDex();
        
        //refresh
        info = saveData->getHeroInfo();
        inkLabel->setString(CCString::createWithFormat("%ld", info.ink)->getCString());
        statusLabel[2]->setString(CCString::createWithFormat("%d", info.Dex)->getCString());
        statusAddLabel[2]->setString(CCString::createWithFormat("%d", getStatusAmount(info.Dex))->getCString());
    }
}


void SGUpgradeScene::luckUp( CCObject* pSender )
{
    SGHeroInfo info = saveData->getHeroInfo();
    int inkAmount = getStatusAmount(info.Luck);
    
    if(info.ink >= inkAmount) {
        saveData->changeInk(-inkAmount);
        saveData->addStatusLuck();
        
        //refresh
        info = saveData->getHeroInfo();
        inkLabel->setString(CCString::createWithFormat("%ld", info.ink)->getCString());
        statusLabel[3]->setString(CCString::createWithFormat("%d", info.Luck)->getCString());
        statusAddLabel[3]->setString(CCString::createWithFormat("%d", getStatusAmount(info.Luck))->getCString());
    }
}

void SGUpgradeScene::exit_Upgrade( CCObject* pSender )
{
	CCScene *scene = SketchTitleScene::create();
	CCDirector::sharedDirector()->replaceScene(scene);
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

SGUpgradeScene::~SGUpgradeScene() {

}
