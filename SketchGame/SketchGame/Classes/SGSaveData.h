//
//  SGSavaData.h
//  SketchGame
//
//  Created by Jung Hanter on 12. 9. 11..
//
//

#ifndef __SketchGame__SGSavaData__
#define __SketchGame__SGSavaData__

#include "cocos2d.h"
#include "SGHero.h"
#include "SGOptionScene.h"

using namespace cocos2d;

class SGSaveData : public CCObject
{
public:
    SGHeroInfo getHeroInfo();
    void addScore(unsigned long long score);
    unsigned long long changeInk(long long amount);
    int addStatusStr();
    int addStatusCon();
    int addStatusDex();
    int addStatusLuck();
    
    SGOptionInfo getOptionInfo();
    void setVibrationOption(bool bVibration);
    void setSoundOption(bool bSound);
    
    void initializeSaveData();
    
private:
    SGHeroInfo heroInfo;
    SGOptionInfo optionInfo;
    CCUserDefault* userDefault;
    
public:
    static SGSaveData* sharedSaveData();
    static SGOptionInfo sharedOptionInfo();
    static SGHeroInfo sharedHeroInfo();
    virtual ~SGSaveData();
    
    
private:
    static SGSaveData* saveData;
    SGSaveData();
    int alignScores(long long newScore);
};


#endif /* defined(__SketchGame__SGSavaData__) */
