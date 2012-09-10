#pragma once
#include "cocos2d.h"
#include "defines.h"


using namespace std;
class SGSignScene : public cocos2d::CCLayer
{
private:
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	vector<cocos2d::CCPoint> points;
	cocos2d::CCSprite *Ink;
	int Height;
	bool OnDraw;
public:
	SGSignScene(void);
	~SGSignScene(void);

	static cocos2d::CCScene *scene();
	bool init();
	void draw();
	LAYER_CREATE_FUNC(SGSignScene);

};
