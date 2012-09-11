#pragma once

#include <cocos2d.h>

using namespace cocos2d;

class CCSpriteAnimation
{
private:
	CCSprite *sprite;
	CCAnimation *animation;
	CCAnimate *animate;
	CCAction *action;

	static CCAnimation *CreateAnimation(CCSprite *,int ,int );

public:
	CCSpriteAnimation(void);
	~CCSpriteAnimation(void);

	static CCSpriteAnimation *create(const char *,int ,int ,float ,bool = true);
	static CCSpriteAnimation *create(CCTexture2D *,int ,int ,float ,bool = true);

	CCSprite *getSprite(){
		return sprite;
	}
	CCAnimation *getAnimation(){
		return animation;
	}
	CCAction *getAction(){
		return action;
	}

	void run(){
		action = this->sprite->runAction(this->animate);
	}
	void stop(){
		action->stop();
	}

};

