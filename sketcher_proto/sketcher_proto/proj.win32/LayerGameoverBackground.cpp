#include "LayerGameoverBackground.h"

#include <math.h>

CLayerGameoverBackground::CLayerGameoverBackground(void)
{
	printf("initialize gameover background layer\n");

	pBgi = CCSprite::create("gameover_background.jpg");
	pBgi->setPosition(ccp(480,320));
	pGlow = CCSprite::create("gameover_glow.png");
	pGlow->setPosition(ccp(480,320));
	pGameover = CCSprite::create("gameover_gameover.png");
	pGameover->setPosition(ccp(480,320));
	pGameover2 = CCSprite::create("gameover_gameover.png");
	pGameover2->setPosition(ccp(480,320));
	pEffect = CCSprite::create("gameover_effect.png");
	pEffect->setPosition(ccp(480,320));

	pEffect->setScale(15);

	ccBlendFunc multiply,add,custom;
	multiply.dst = GL_ONE_MINUS_SRC_ALPHA;
	multiply.src = GL_DST_COLOR; 
	custom.src = GL_SRC_COLOR;
	custom.dst = GL_ONE_MINUS_SRC_COLOR;
	add.src = GL_SRC_ALPHA;
	add.dst = GL_ONE;
	

	pGlow->setBlendFunc(multiply);
	pGlow->setOpacity(0);
	pGameover->setBlendFunc(custom);
	pGameover2->setBlendFunc(custom);
	pEffect->setBlendFunc(add);
	pEffect->setOpacity(0);

	pGameover2->setOpacity(0);

	this->addChild(pBgi);
	this->addChild(pGameover);
	this->addChild(pGlow);
	this->addChild(pGameover2);
	this->addChild(pEffect);
	
	

	this->schedule(schedule_selector(CLayerGameoverBackground::update),0.01f);

	animation = 0;
}
CLayerGameoverBackground::~CLayerGameoverBackground(void)
{
}

void CLayerGameoverBackground::update(float dt){
	
	if(animation != 255){
		animation ++;
		pGlow->setOpacity(animation);

		if(pEffect->getOpacity() != 150){
			pEffect->setOpacity(pEffect->getOpacity()+1);
			pEffect->setRotation(pEffect->getRotation()+0.24);
		}
	}
	else{
		if(pGameover2->getOpacity() != 255){
			pGameover2->setOpacity(pGameover2->getOpacity() + 1);
			float nx,ny;
			nx = pGameover2->getPositionX() - 12.0f/255.0f;
			ny = pGameover2->getPositionY() + 12.0f/255.0f;
			pGameover2->setPosition(ccp(nx,ny));
		}
		else
			this->unschedule(schedule_selector(CLayerGameoverBackground::update));

	
	}
	
}
