#include "SceneAbout.h"

CLayerAboutMenu *CSceneAbout::pLayerMenu = NULL;
CLayerAboutBackground *CSceneAbout::pLayerBackground = NULL;

CCScene* CSceneAbout::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CSceneAbout *layer = CSceneAbout::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}
CSceneAbout::CSceneAbout(void)
{
}
CSceneAbout::~CSceneAbout(void)
{
}

bool CSceneAbout::init(){
	CCLayer::init();
	return true;
}
void CSceneAbout::onEnter(){
	CCLayer::onEnter();

	pLayerBackground = new CLayerAboutBackground();
	pLayerMenu = new CLayerAboutMenu();

	this->addChild(pLayerBackground);
	this->addChild(pLayerMenu);
}
