#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class SGSound : public CocosDenshion::SimpleAudioEngine
{ 
public:
	SGSound(void);
	~SGSound(void);
	void playBackgroundMusic(const char* Filepath,bool loop);
	void playEffect(const char *Filepath,bool loop);
};
