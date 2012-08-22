#include "CSpriteAnimation.h"


CCSpriteAnimation::CCSpriteAnimation(void)
{
}


CCSpriteAnimation::~CCSpriteAnimation(void)
{
}

CCAnimation *CCSpriteAnimation::CreateAnimation(CCSprite *sprite,int w,int h){
	CCAnimation *ani = CCAnimation::create();

	int widthPerUnit = sprite->getContentSize().width / w;
	int heightPerUnit = sprite->getContentSize().height / h;
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			ani->addSpriteFrameWithTexture(
				sprite->getTexture(),
				CCRectMake(
					j*widthPerUnit,
					i*heightPerUnit,
					widthPerUnit,
					heightPerUnit
				)
				);
		}
	}
	return ani;
}

CCSpriteAnimation *CCSpriteAnimation::create(
		const char *file,
		int w,int h,
		float delay,
		bool repeat
	){
	
	;
	return CCSpriteAnimation::create(
		CCSprite::create(file)->getTexture(),
		w,h,
		delay,
		repeat);
}
CCSpriteAnimation *CCSpriteAnimation::create(
		CCTexture2D *texture,
		int w,int h,
		float delay,
		bool repeat
	){
	CCSpriteAnimation *sa = (CCSpriteAnimation*)malloc(sizeof(CCSpriteAnimation));

	sa->sprite = CCSprite::create(texture);
	sa->animation = CreateAnimation(sa->sprite,w,h);
	sa->animation->setDelayPerUnit(delay);
	sa->animate = CCAnimate::actionWithAnimation(sa->animation);
	if(repeat){
		sa->animate = (CCAnimate*)CCRepeatForever::actionWithAction(sa->animate);
	}

	return sa;
}