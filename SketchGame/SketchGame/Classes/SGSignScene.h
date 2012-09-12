#pragma once
#include "cocos2d.h"
#include "defines.h"

using namespace cocos2d;
using namespace std;
class SGSignScene : public cocos2d::CCLayer
{
private:
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	vector<cocos2d::CCPoint> points;
	cocos2d::CCSprite *Ink;

	void update(float);
	int Height;
	int Height_sum;
	bool OnDraw;

	CCSprite* ink_bottle;
	CCActionInterval* ink_bottle_act[5];
    CCActionInterval* ink_bottle_no_act;

	CCRenderTexture *rt;	
	CCSprite *sign_sprite;
	CCTexture2D * sign_texture;
	bool init_ani;

	void saveSign(CCObject* pSender);
public:
	SGSignScene(void);
	virtual ~SGSignScene(void);

	static cocos2d::CCScene *scene();
	bool init();
	void draw();
	LAYER_CREATE_FUNC(SGSignScene);

};
