#include "SGSound.h"
#include "SGOptionScene.h"

extern bool onSound2;

SGSound::SGSound(void)
{
}

SGSound::~SGSound(void)
{
}

void SGSound::playBackgroundMusic( const char* Filepath,bool loop )
{
	onSound2;
	if(onSound2 ==true)
		playBackgroundMusic(Filepath,loop);
}

void SGSound::playEffect( const char *Filepath,bool loop )
{
	if(onSound2 == true)
		playEffect(Filepath,loop);
}

