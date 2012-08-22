#include "SceneTitle.h"

#include "flags.h"

CLayerTitleMenu *CSceneTitle::pLayerMenu = NULL;
CLayerTitleBackground *CSceneTitle::pLayerBackground = NULL;

EXTERN_FLAG;

CCScene* CSceneTitle::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CSceneTitle *layer = CSceneTitle::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool CSceneTitle::init(){
	printf("initialize title scene\n");

	CCLayer::init();

	sprite2 = CCSprite::create("eraser.png");
	sprite = CCSprite::create("eraser_real.png");
	sprite2->setAnchorPoint(ccp(0,1));
	
	this->addChild(sprite);
	this->addChild(sprite2);

	return true;
}
void CSceneTitle::onEnter(){
	printf("enter title scene\n");

	CCLayer::onEnter();

	erasing = false;

	pLayerBackground = new CLayerTitleBackground();
	pLayerMenu = new CLayerTitleMenu();

	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	addChild(pLayerBackground);
	addChild(pLayerMenu);
}
void CSceneTitle::erase(){
	erasing = true;

	pLayerMenu->setVisible(false);
	pLayerBackground->setVisible(false);

	eraserXY.x = 0;
	eraserXY.y = 640;

	dir = 0;

	this->schedule(schedule_selector(CSceneTitle::cbErase));
}
void CSceneTitle::cbErase(float dt){
	printf("OnErase\n");

	int cnt = 5;
	for(int i=0;i<cnt;i++){
		xy.push_back(eraserXY);
		xyOpa.push_back(0);
		if(dir == 0){
			eraserXY.x += 30;
			eraserXY.y += 30;

			if(eraserXY.y >= 700) dir++;
		}
		else if(dir == 1){
			eraserXY.x += 30;
			dir++;
		}
		else if(dir == 2){
			eraserXY.x -= 30;
			eraserXY.y -= 30;
			if(eraserXY.y <= -30) dir++;
			if(eraserXY.x <= -70) dir++;
		}
		else if(dir == 3){
			eraserXY.x += 30;
			dir = 0;
		}
	}
}

void CSceneTitle::draw(){
	printf("OnDraw\n");

	CCLayer::draw();

	if(erasing){
		pLayerMenu->setVisible(true);
		pLayerBackground->setVisible(true);

		pLayerBackground->visit();
		pLayerMenu->visit();
		
		int i;
		for(i=0;i<xy.size();i++){
			sprite2->setOpacity(xyOpa[i]);
			sprite2->setPosition(ccp(xy[i].x,xy[i].y));
			sprite2->visit();

			xyOpa[i] ++;
		}

		if(i >= 3){
			i = xy.size()-1;
			sprite->setOpacity(255);
			sprite->setPosition(ccp(xy[i].x+20,xy[i].y-20));
			sprite->visit();
		}

		pLayerMenu->setVisible(false);
		pLayerBackground->setVisible(false);
	}
}
