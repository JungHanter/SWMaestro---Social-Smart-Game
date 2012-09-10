#include "SGSound.h"
#include "SGOptionScene.h"

SGSound::SGSound(void)
{
    onSound2 = true;
}

SGSound::~SGSound(void)
{
}

void SGSound::playBackgroundMusic( const char* Filepath,bool loop )
{
    //onSound2;
	if(onSound2 ==true)
		playBackgroundMusic(Filepath,loop);
}

void SGSound::playEffect( const char *Filepath,bool loop )
{
	if(onSound2 == true)
		playEffect(Filepath,loop);
}


bool SGSound::isSound2() {
    return onSound2;
}

void SGSound::setSound2(bool _bSound) {
    onSound2 = _bSound;
}