
#ifndef __NEW_SEASON_GUIDE_H__
#define __NEW_SEASON_GUIDE_H__

#include "BaseGuide.h"

using namespace cocos2d;

class NewSeasonGuide : public BaseGuide{
public:
	//创建一个Logo的场景
	static CCScene* createGuideScene(int seasonId,std::string baseFileName,int guidePicNum);
private:
	NewSeasonGuide(std::string guidePicBaseName,int picNum);
	//如果引导结束有不一样的操作，那么一定要重写！
	virtual void guideEnd();

	int seasonId;
};

#endif