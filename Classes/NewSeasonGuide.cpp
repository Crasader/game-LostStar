#include "NewSeasonGuide.h"
#include "GameInforEditer.h"
#include "LevelManager.h"

NewSeasonGuide::NewSeasonGuide(std::string guidePicBaseName,int picNum):BaseGuide(guidePicBaseName,picNum)
{
	seasonId = -1;
}

CCScene* NewSeasonGuide::createGuideScene(int seasonId,std::string baseFileName,int guidePicNum)
{
	CCScene* scene = CCScene::create();
	NewSeasonGuide* guide = new NewSeasonGuide(baseFileName,guidePicNum);
	guide->autorelease();
	scene->addChild(guide);

	guide->seasonId = seasonId;
	return scene;
}

void NewSeasonGuide::guideEnd()
{
	LevelManager::getManager()->setShowLoadingAnimation(true);
	//直接去第一关
	LevelManager::getManager()->goTo(seasonId,1);

	SeasonInfor* seasonInfor = GameInforEditer::getGameInfor()->getSeasonInfor(seasonId);
	if (seasonInfor == NULL)
	{
		seasonInfor = new SeasonInfor();
		seasonInfor->seasonId = seasonId;
		seasonInfor->isPlayed = true;//设置成已经玩过
		seasonInfor->locked = false;
		seasonInfor->score = 0;

		GameInforEditer::getGameInfor()->addSeasonInfor(seasonInfor);
	}else
	{
		seasonInfor->isPlayed = true;
	}

	//保存
	GameInforEditer::getGameInfor()->saveSessonInforList();
}