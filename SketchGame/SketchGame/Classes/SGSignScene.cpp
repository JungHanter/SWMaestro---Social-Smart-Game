#include "SGSignScene.h"
#include "SketchTitleScene.h"

using namespace cocos2d;
SGSignScene::SGSignScene(void)
{
}

SGSignScene::~SGSignScene(void)
{
}
CCScene* SGSignScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		// 'layer' is an autorelease 
		cocos2d::CCLayer *layer = SGSignScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}

bool SGSignScene::init()
{
    if(CCLayer::init()) {
        setTouchEnabled(true);
        OnDraw = true;
        Height = 0;
        Height_sum = 0;
        init_ani = false;

        CCSpriteFrameCache *pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        pSpriteFrameCache->addSpriteFramesWithFile("game_etc.plist", "game_etc.png");


        CCSprite* background = CCSprite::create("sign_background.png");
        background->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, 
            CCDirector::sharedDirector()->getWinSize().height/2));
        this->addChild(background,-1);


        ink_bottle = CCSprite::create(pSpriteFrameCache->spriteFrameByName("ink_bottle.png"));
        ink_bottle->retain();
        ink_bottle->setPosition(ccp(100,200));
        this->addChild(ink_bottle);
        for(int i=0; i<5; i++) {
            CCArray* pInkWaveFrames = CCArray::create();
            for(int j=1; j<=3; j++) {
                pInkWaveFrames->addObject(pSpriteFrameCache->spriteFrameByName(CCString::createWithFormat("ink_step%d_%d.png", i+1, j)->getCString()));
            }
            ink_bottle_act[i] = CCRepeatForever::create(CCAnimate::create(CCAnimation::create(pInkWaveFrames, GAME_FRAME_SPEED)));
            ink_bottle_act[i]->retain();
            pInkWaveFrames->release();
        }
        this->schedule(schedule_selector(SGSignScene::update));
        
        ink_bottle->runAction(ink_bottle_act[4]);
        
        CCArray* pInkNoAct = CCArray::create();
        pInkNoAct->addObject(pSpriteFrameCache->spriteFrameByName("ink_bottle.png"));   
        ink_bottle_no_act = CCAnimate::create(CCAnimation::create(pInkNoAct, GAME_FRAME_SPEED));
        ink_bottle_no_act->retain();
        pInkNoAct->release();
        

        rt = CCRenderTexture::create(CCDirector::sharedDirector()->getWinSize().width,CCDirector::sharedDirector()->getWinSize().height);

            
        rt->clear(0,0, 0,0);
        rt->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,
            CCDirector::sharedDirector()->getWinSize().height/2));
        this->addChild(rt, -1);

        CCMenuItemImage *SaveImage = CCMenuItemImage::create("back.png","back_p.png",this,
            menu_selector(SGSignScene::saveSign));
        SaveImage->setRotation(180.f);
        SaveImage->setPosition(ccp(420,300));
        CCMenu* pSaveSign = CCMenu::create(SaveImage, NULL);
        pSaveSign->setPosition(CCPointZero);
        this->addChild(pSaveSign, 1);
        return true;
    }
    return false;
}

void SGSignScene::update(float)
{
	if(Height >= 20)
	{
		Height_sum += Height;
		Height =0;
		init_ani = false;
	}
	if(init_ani == false)
	{	
		if(Height_sum >= 120)
		{
			ink_bottle->stopAllActions();
            ink_bottle->runAction(ink_bottle_no_act);
			init_ani = true;
			OnDraw = false;
		}
		if(Height_sum >= 100 )
		{
			ink_bottle->stopAllActions();
			ink_bottle->runAction(ink_bottle_act[0]);
			init_ani = true;
		}
		else if(Height_sum >= 80 )
		{
			ink_bottle->stopAllActions();
			ink_bottle->runAction(ink_bottle_act[1]);
			init_ani = true;
		}
		else if(Height_sum >= 60 )
		{
			ink_bottle->stopAllActions();
			ink_bottle->runAction(ink_bottle_act[2]);
			init_ani = true;
		}
		else if(Height_sum >= 40)
		{
			ink_bottle->stopAllActions();
			ink_bottle->runAction(ink_bottle_act[3]);
			init_ani = true;
		}
		else if(Height_sum >= 20)
		{
			ink_bottle->stopAllActions();
			ink_bottle->runAction(ink_bottle_act[4]);
			init_ani = true;
		}
	}
}

void SGSignScene::draw()
{   
	glLineWidth(5);	
	for (int i = 0; i < points.size(); i++) 
	{
		//glColor4ub(0,30,255,0);
		ccDrawColor4B(0,0,0,255);
		CCPoint p = points.at(i);
		//ccDrawPoint(p);
		CCPoint *vertices = &points[0]; 
		ccDrawPoly(vertices, points.size(), false);
	}
}

void SGSignScene::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());

	rt->clear(0,0,0,0);
	Height = 0;
	Height_sum = 0;
	init_ani = false;
	points.clear();
	OnDraw = true;
	if(OnDraw == true)
		points.push_back(location);
}

void SGSignScene::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());

	if(OnDraw == true)
		points.push_back(location);

	if(Height_sum < 120)
		Height +=1;
}
void SGSignScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch-> locationInView());
}

void SGSignScene::saveSign( CCObject* pSender )
{
	rt->begin();
	this->visit();
	rt->end();
	sign_texture = rt->getSprite()->getTexture();
	sign_sprite = CCSprite::create(sign_texture);\
	//a->setScaleY(-1);
	//this->addChild(a,-2);
    
    CCDirector::sharedDirector()->replaceScene(SketchTitleScene::create());
}
