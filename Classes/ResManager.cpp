#include "ResManager.h"
#include "animatepacker/AnimatePacker.h"
#include "cocos-ext.h"
#include "AchievementManager.h"
#include "LFStrings.h"
#include "GameInforEditer.h"
USING_NS_CC_EXT;

static ResManager* _pManager = NULL;

ResManager::ResManager()
{
	m_eLanguageType = getDiviceLanguage();
}

ResManager* ResManager::getManager()
{
	if (_pManager == NULL)
	{
		_pManager = new ResManager();
	}
	return _pManager;
}

ResManager::~ResManager()
{
	CC_SAFE_DELETE(_pManager);
}

string ResManager::getFilePath(string filename, bool diffrentLanguage)
{
	if (diffrentLanguage){
		return getFilePathByLanguageType(filename, m_eLanguageType);
	}else{
		return getSharedFilePath(filename);
	}
}

string ResManager::getSharedFilePath(string fileName)
{
	return "shared/"+fileName;
}

string ResManager::getSharedFilePath()
{
	return "shared/";
}

string ResManager::getFilePathByLanguageType(string fileName)
{
	return getFilePathByLanguageType(fileName, m_eLanguageType);
}

string ResManager::getFilePathByLanguageType(string fileName, LanguageType eLanguageType)
{
	return getLanguagePathString(eLanguageType)+fileName;
}

void ResManager::setLanguageType(LanguageType eLanguageType)
{
	m_eLanguageType = eLanguageType;
	CCLog("ResManager::%s = [%s]", __FUNCTION__, getLanguagePathString(m_eLanguageType).c_str());
}

LanguageType ResManager::getLanguageType()
{
	return m_eLanguageType;
}

string ResManager::getLanguagePathString()
{
	return getLanguagePathString(m_eLanguageType);
}

string ResManager::getLanguagePathString(LanguageType eLanguageType)
{
	string ret = "en";
	switch (eLanguageType)
	{
	case kLanguageEnglish:
		ret = "en";
		break;
	case kLanguageChinese:
		ret = "zh";
		break;
	case kLanguageFrench:
		ret = "fr";
		break;
	case kLanguageGerman:
		ret = "ge";
		break;
	case kLanguageItalian:
		ret = "it";
		break;
	case kLanguageRussian:
		ret = "ru";
		break;
	case kLanguageSpanish:
		ret = "sp";
		break;
	case kLanguageJapanese:
		ret = "jp";
		break;
	case kLanguageKorean:
		ret = "ko";
		break;
	case kLanguagePortuguese:
		ret = "po";
		break;
	case kLanguageHungarian:
		ret = "hu";
		break;
	case kLanguageArabic:
		ret = "ar";
		break;
	}
	return ret+"/";
}

LanguageType ResManager::getDiviceLanguage()
{
	LanguageType tyep = CCApplication::sharedApplication()->getCurrentLanguage();
	if (tyep != kLanguageGerman)
	{
		return kLanguageEnglish;
	}
	return tyep;
}

void ResManager::reloadResource()
{
	removeAllAnimationCache();
	removeAllArmatureInfo();
	removeAllSpriteFrameCache();
	removeAllTextureCache();

	LFStrings::init();
	AchievementManager::getInstance()->reloadAchievementDefaultInfo();
	GameInforEditer::getGameInfor()->saveSessonInforList();
}

void ResManager::removeAllTextureCache()
{
	CCTextureCache::sharedTextureCache()->removeAllTextures();
}

void ResManager::removeAllSpriteFrameCache()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
}

void ResManager::removeAllAnimationCache()
{
	CCAnimationCache::sharedAnimationCache()->purgeSharedAnimationCache();
}

string ResManager::getShadowPath(string path)
{
	string p = path;
	//replace_all_distinct(p,"_no.png",".png"); // 不在让不可消除的也用这个图片了，每一个都用不同的图片
	replace_all_distinct(p,".png","_shadow.png");
	return p;
}

void ResManager::removeAllArmatureInfo()
{
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("star_r1.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("STAR_FALL_UI_jiaoxue.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("star_r.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("starfall.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("boom.ExportJson");
}

void ResManager::loadAllArmatureInfo()
{
	removeAllArmatureInfo();

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("star_r1.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("STAR_FALL_UI_jiaoxue.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("star_r.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("starfall.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("boom.ExportJson");
}
