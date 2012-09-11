#include "SceneGameover.h"

CLayerGameoverMenu *CSceneGameover::pLayerMenu = NULL;
CLayerGameoverBackground *CSceneGameover::pLayerBackground = NULL;


CCScene* CSceneGameover::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CSceneGameover *layer = CSceneGameover::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool CSceneGameover::init(){
	printf("initialize gameover scene\n");

	CCLayer::init();

	return true;
}
void CSceneGameover::onEnter(){
	printf("enter gameover scene\n");

	CCLayer::onEnter();

	pLayerBackground = new CLayerGameoverBackground();
	pLayerMenu = new CLayerGameoverMenu();

	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	addChild(pLayerBackground);
	addChild(pLayerMenu);
}