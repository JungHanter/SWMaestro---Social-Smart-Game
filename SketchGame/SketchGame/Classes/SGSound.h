#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class SGSound : public CocosDenshion::SimpleAudioEngine
{
    
private:
    bool onSound2;
    
public:
	SGSound(void);
	~SGSound(void);
	void playBackgroundMusic(const char* Filepath,bool loop);
	void playEffect(const char *Filepath,bool loop);
    

    bool isSound2();
    void setSound2(bool _bSound);
};
