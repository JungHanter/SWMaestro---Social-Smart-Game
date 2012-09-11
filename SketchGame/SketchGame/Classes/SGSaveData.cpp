//
//  SGSavaData.cpp
//  SketchGame
//
//  Created by Jung Hanter on 12. 9. 11..
//
//

#include "SGSaveData.h"

SGSaveData* SGSaveData::saveData = NULL;

SGHeroInfo SGSaveData::getHeroInfo() {
    return heroInfo;
}

void SGSaveData::addScore(unsigned long long score) {
    int newPos = alignScores(score);
    
    for(int i=newPos; i<10; i++) {
        userDefault->setIntegerForKey(CCString::createWithFormat("SCORE%d",i)->getCString(),
                                      heroInfo.score[i]);
    }
    userDefault->flush();
    
}

unsigned long long SGSaveData::changeInk(long long amount) {
    heroInfo.ink += amount;
    userDefault->setIntegerForKey("INK", heroInfo.ink);
    userDefault->flush();
    return heroInfo.ink;
}

int SGSaveData::addStatusStr() {
    heroInfo.Str++;
    userDefault->setIntegerForKey("STR", heroInfo.Str);
    userDefault->flush();
    return heroInfo.Str;
}

int SGSaveData::addStatusCon() {
    heroInfo.Con++;
    userDefault->setIntegerForKey("CON", heroInfo.Con);
    userDefault->flush();
    return heroInfo.Con;
}

int SGSaveData::addStatusDex() {
    heroInfo.Dex++;
    userDefault->setIntegerForKey("DEX", heroInfo.Dex);
    userDefault->flush();
    return heroInfo.Dex;
}

int SGSaveData::addStatusLuck() {
    heroInfo.Luck++;
    userDefault->setIntegerForKey("LUCK", heroInfo.Luck);
    userDefault->flush();
    return heroInfo.Luck;
}

SGOptionInfo SGSaveData::getOptionInfo() {
    return optionInfo;
}

void SGSaveData::setVibrationOption(bool bVibration) {
    optionInfo.bVibration = bVibration;
    userDefault->setBoolForKey("VIB", optionInfo.bVibration);
    userDefault->flush();
}

void SGSaveData::setSoundOption(bool bSound) {
    optionInfo.bSound = bSound;
    userDefault->setBoolForKey("SND", optionInfo.bSound);
    userDefault->flush();
}


void SGSaveData::initializeSaveData() {
    heroInfo.ink = 0;
    for(int i=0; i<10; i++) {
        heroInfo.score[i] = 0;
    }
    heroInfo.Str = heroInfo.Con = heroInfo.Dex = heroInfo.Luck = 10;
    optionInfo.bSound = optionInfo.bVibration = true;
    
    userDefault->setIntegerForKey("STR", 10);
    userDefault->setIntegerForKey("CON", 10);
    userDefault->setIntegerForKey("DEX", 10);
    userDefault->setIntegerForKey("LUCK", 10);
    userDefault->setIntegerForKey("INK", 0);
    for(int i=0; i<10; i++) {
         userDefault->setIntegerForKey(CCString::createWithFormat("SCORE%d",i)->getCString(), 0);
    }
    
    userDefault->setBoolForKey("SND", true);
    userDefault->setBoolForKey("VIB", true);
    
    userDefault->flush();
}


SGSaveData* SGSaveData::sharedSaveData() {
    if(saveData == NULL) {
        saveData = new SGSaveData();
        saveData->retain();
    }
    
    return saveData;
}

SGOptionInfo SGSaveData::sharedOptionInfo() {
    return sharedSaveData()->getOptionInfo();
}

SGHeroInfo SGSaveData::sharedHeroInfo() {
    return sharedSaveData()->getHeroInfo();
}

//return newScore's pos
int SGSaveData::alignScores(long long newScore) {
    //no refresh
    if(newScore <= heroInfo.score[9]) {
        return -1;
    }
    
    
    heroInfo.score[9] = newScore;

    //decrease align
    int tmp;
    for(int i=8; i>=0; i--) {
        if(heroInfo.score[i] < heroInfo.score[i+1]) {
            tmp = heroInfo.score[i];
            heroInfo.score[i] = heroInfo.score[i+1];
            heroInfo.score[i+1] = tmp;
        } else {
            return i+1;
        }
    }
    
    return 0;
}

SGSaveData::SGSaveData() {
    userDefault = CCUserDefault::sharedUserDefault();
    
    heroInfo.Str = userDefault->getIntegerForKey("STR", 10);
    heroInfo.Con = userDefault->getIntegerForKey("CON", 10);
    heroInfo.Dex = userDefault->getIntegerForKey("DEX", 10);
    heroInfo.Luck = userDefault->getIntegerForKey("LUCK", 10);
    heroInfo.ink = userDefault->getIntegerForKey("INK", 0);
    for(int i=0; i<10; i++) {
        heroInfo.score[i] = userDefault->getIntegerForKey(CCString::createWithFormat("SCORE%d",i)->getCString(), 0);
    }
    
    optionInfo.bSound = userDefault->getBoolForKey("SND", true);
    optionInfo.bVibration = userDefault->getBoolForKey("VIB", true);
    
    userDefault->flush();
}

SGSaveData::~SGSaveData() {
    saveData = NULL;
}
