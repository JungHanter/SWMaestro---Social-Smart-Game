//
//  SGRankingScene.h
//  SketchGame
//
//  Created by Jung Hanter on 12. 9. 12..
//
//

#ifndef __SketchGame__SGRankingScene__
#define __SketchGame__SGRankingScene__

#include "cocos2d.h"
#include "SGSavaData.h"
using namespace cocos2d;

class SGRankingLayer : public cocos2d::CCLayerColor
{
private:
    void back();
    
private:
    CCLabelAtlas* scoreLabel[10];
    
public:
	SGRankingLayer(void) {};
	virtual ~SGRankingLayer(void);
    
	//static SGHeroInfo getUpgradeInfo();
	static cocos2d::CCScene *scene();
	bool init();
    LAYER_CREATE_FUNC(SGRankingLayer);
};

#endif /* defined(__SketchGame__SGRankingScene__) */
