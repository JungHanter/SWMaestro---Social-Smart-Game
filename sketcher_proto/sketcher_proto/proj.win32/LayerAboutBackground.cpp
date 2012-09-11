#include "LayerAboutBackground.h"


CLayerAboutBackground::CLayerAboutBackground(void)
{
}
CLayerAboutBackground::~CLayerAboutBackground(void)
{
}
bool CLayerAboutBackground::init(){
	CCLayer::init();
	return true;
}
void CLayerAboutBackground::onEnter(){
	CCLayer::onEnter();

	pBgi = CCSprite::create("title_background_wb.jpg");
	pBgi->setPosition(
		ccp(480,320)
		);

	CCLabelTTF *pLabel = CCLabelTTF::create("About","Arial",35);

	pLabel->setPosition(ccp(480,320));
	pLabel->setColor(ccc3(255,0,0));

	this->addChild(pBgi);
	this->addChild(pLabel);
}