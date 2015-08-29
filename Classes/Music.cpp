#include "Music.h"
#include "SpriteHelper.h"
#include "LevelManager.h"

#define Music_Bg_Close		0

// 音效的根目录
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
static string dirRoot = "";
#else
static string dirRoot = "sounds/";
#endif

Music* Music::musicInstance;
Music::Music()
{
	simpleAudioEngine = SimpleAudioEngine::sharedEngine();
	initBgVolume = 1.0f;
	initEffectVolume = 1.0f;
	preloadEffectSounds.clear();
	type = MusicType_NULL;
	setMute(false);
	CCLog("%s --> construct over",__FUNCTION__);
	m_iLastPlaySeasondBgMusicId = 0;
	m_bBeforeBackGroundBackgroundMute = false;
	m_bBeforeBackGroundEffectMute = false;
}

Music* Music::sharedMusicPlayer()
{
	if (musicInstance == NULL)
	{
		musicInstance = new Music();
	}
	return musicInstance;
}
//void Music::gamePause()
//{
//	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
//}
//void Music::gameResume()
//{
//	sharedMusicPlayer()->setBackgroundMute(sharedMusicPlayer()->isBackgroundMute());
//}
void Music::safeRelease()
{
	std::list<std::string>::iterator it;
	for (it=preloadEffectSounds.begin(); it!=preloadEffectSounds.end(); ++it) 
	{
		std::string fileName = (*it);
		simpleAudioEngine->unloadEffect(fileName.c_str());
	}
	preloadEffectSounds.clear();

	delete musicInstance;

	musicInstance = NULL;
}
// ============================ 游戏常用函数 =============================//
void Music::playMainMusic()
{
#if Music_Bg_Close != 0
	return;
#endif
	resumeBackgroundSound();
//	if (type == MusicType_Main)
//		return;

	playBackgroundSound("bg_music_main.mp3");
	type = MusicType_Main;
}
void Music::playPlayMusic()
{
#if Music_Bg_Close != 0
	return;
#endif
	resumeBackgroundSound();
//	if (type == MusicType_Play)
//		return;
	
	type = MusicType_Play;

	if (m_iLastPlaySeasondBgMusicId == LevelManager::getManager()->getLastSeasonId())
	{
		return;
	}
	setLastPlaySeasondBgMusicId(LevelManager::getManager()->getLastSeasonId());
	playBackgroundSound(CCString::createWithFormat("bg_music_play-%d.mp3", m_iLastPlaySeasondBgMusicId)->getCString());
}
//
void Music::buttonClicked()
{
	playEffectSound("button.wav");
}
// ============================ 游戏常用函数 =============================//



void Music::playBackgroundSound(std::string fileName,bool loop /* = true*/)
{
#if Music_Bg_Close != 0
	return;
#endif
	fileName = dirRoot + fileName;

	simpleAudioEngine->playBackgroundMusic( fileName.c_str(),loop);
	setBackgroundMute(backgroudMute);
}
void Music::pauseBackgroundSound()
{ 
	simpleAudioEngine->pauseBackgroundMusic();
}
void Music::pauseAllEffects()
{
	simpleAudioEngine->pauseAllEffects();
}
void Music::resumeAllEffects()
{
	simpleAudioEngine->resumeAllEffects();
}

void Music::resumeBackgroundSound()
{
	simpleAudioEngine->resumeBackgroundMusic();
}
void Music::stopBackgroundSound(bool release)
{
	type = MusicType_NULL;
	simpleAudioEngine->stopBackgroundMusic(release);
}

void Music::stopAllEffect()
{
	simpleAudioEngine->stopAllEffects();
}
void Music::loadEffectSound(std::string fileName)
{
	fileName = dirRoot + fileName;

	list<string>::iterator it = preloadEffectSounds.begin();
	for (;it != preloadEffectSounds.end();it++)
	{
		if ((*it) == fileName)
		{
			// 已经载入过了
			return;
		}
	}
	//CCLog("Music::loadEffectSound---->[%s]",fileName.c_str());
	simpleAudioEngine->preloadEffect(fileName.c_str());
	preloadEffectSounds.insert(preloadEffectSounds.end(),fileName);
}
size_t Music::playEffectSound(std::string fileName,bool loop/* = false*/)
{
	fileName = dirRoot + fileName;
	//loadEffectSound(fileName);
	return simpleAudioEngine->playEffect(fileName.c_str(),loop);
}
void Music::stopEffectSound(size_t nSoundId)
{
	simpleAudioEngine->stopEffect(nSoundId);
}
void Music::pauseEffectSound(size_t nSoundId)
{
	simpleAudioEngine->pauseEffect(nSoundId);
}
void Music::resumeEffectiveSound(size_t nSoundId)
{
	simpleAudioEngine->resumeEffect(nSoundId);
}
void Music::unLoadEffectSound(std::string fileName)
{
	fileName = dirRoot + fileName;

    simpleAudioEngine->unloadEffect(fileName.c_str());

	list<string>::iterator it = preloadEffectSounds.begin();
	for (;it != preloadEffectSounds.end();it++)
	{
		if ((*it) == fileName)
		{
			preloadEffectSounds.erase(it);
			break;
		}
	}
}
void Music::unLoadAllEffectSound()
{
	list<string>::iterator it = preloadEffectSounds.begin();
	for (;it != preloadEffectSounds.end();it++)
	{
		simpleAudioEngine->unloadEffect((*it).c_str());
	}
	preloadEffectSounds.clear();
}

void Music::setMute(bool mute /* = true */)
{
	setBackgroundMute(mute);
	setEffectiveMute(mute);
	CCLog("%s --> mute = %s",__FUNCTION__,mute?"true":"false");
}

void Music::setBackgroundMute(bool mute /* = true */)
{
	backgroudMute = mute;
	if (mute)
		simpleAudioEngine->setBackgroundMusicVolume(0.0f);
	else
		simpleAudioEngine->setBackgroundMusicVolume(initBgVolume);

	this->mute = backgroudMute&&effetiveMute;
	
}
void Music::setEffectiveMute(bool mute /* = true */)
{
	effetiveMute = mute;
	if (mute)
		simpleAudioEngine->setEffectsVolume(0.0f);
	else
		simpleAudioEngine->setEffectsVolume(initEffectVolume);

	this->mute = backgroudMute&&effetiveMute;
}

void Music::setVolume(float volume)
{
	setEffectVolume(volume);
	setBgVolume(volume);
}

void Music::setEffectVolume(float volume)
{
	simpleAudioEngine->setEffectsVolume(volume);
	initEffectVolume = volume;

	//simpleAudioEngine->
}
void Music::setBgVolume(float volume)
{
	simpleAudioEngine->setBackgroundMusicVolume(volume);
	initBgVolume = volume;
}

string Music::getDefaultVolumeFile(string file,float volume)
{
	//file = dirRoot + file;
	string suffix = "";
	int pre_part = (int)volume;
	int end_part = (int)((volume - pre_part )*10);
	
	char format[64];
	sprintf(format,"_%d_%d.wav",pre_part,end_part);

	string newName = SpriteHelper::replace_all_distinct(file,".wav",format);

	return newName;
}
