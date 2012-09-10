#include "SGSignScene.h"

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
	Ink = CCSprite::create("Ink.png");
	Ink->setAnchorPoint(CCPoint (0,1));
	Ink->setPosition(ccp(100,300));
	this->addChild(Ink);
	setTouchEnabled(true);
	OnDraw = true;
	Height = 0;
	return true;
}

void SGSignScene::draw()
{
	//glLineWidth( 5.0f );
	//ccDrawColor4B(0,255,0,255);
	//ccDrawLine(ccp(0,400), ccp(400,0));	
    
    //glPointSize(4);
	glLineWidth(5);
    
	for (int i = 0; i < points.size(); i++) 
	{
		//glColor4ub(255,0,255,255);
		CCPoint p = points.at(i);
		//ccDrawPoint(p);
		CCPoint *vertices = &points[0]; 
		ccDrawPoly(vertices, points.size(), false);

	}
	//for (int i = 0; i < points.size(); i++) 
	//{
	//	//	glColor4ub(0,255,0,255); 
	//	glColor4ub(255,0,255,255); 
	//	CCPoint p = points.at(i);
	//	//ccDrawPoint(p);
	//	CCPoint *vertices = &points[0]; 
	//	int minus=0;
	//	if(start.size() > 1)
	//	{
	//		for(int z=1; z < start.size() ; z ++)
	//		{
	//			int some = z-1;
	//			if(i > start.at(some) && i < start.at(z) && start.empty() == false)
	//			{
	//				vertices = &points_start.at(some); //
	//				minus = start.at(some);
	//			}
	//		}
	//	}
	//	ccDrawPoly(vertices, points.size()-minus, false);
	//}
}

void SGSignScene::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	//b2Vec2 locationWorld = b2Vec2(location.x, location.y);

	//points.clear();	

	if(OnDraw == true)
		points.push_back(location);

	//start.push_back(points.size());
	//points_start.push_back(location);
}

void SGSignScene::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());

	//ccDrawLine(last,location);
	//b2Vec2 locationWorld = b2Vec2(location.x, location.y);
	if(OnDraw == true)
		points.push_back(location);

	if(Height < 200)
	{
		Height +=1;
		
		Ink->setTextureRect(CCRect(0,0,50,200-Height));
	//	Ink->setPosition(ccp(100,300));

		if(Height == 200)
			OnDraw = false;
	}
}
void SGSignScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch *touch = (CCTouch*)(touches->anyObject());
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch-> locationInView());

	//b2Vec2 locationWorld = b2Vec2(location.x, location.y);
}
