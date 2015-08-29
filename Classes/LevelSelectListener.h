#ifndef _LevelSelectListener_H_
#define _LevelSelectListener_H_

#include "cocos2d.h"

using namespace cocos2d;

class LevelSelectListener
{
public:
	//season被选中之后调用
	virtual void seasonSelected(int seasonId,int lastSeasonId,int lastSectionId,CCPoint seasonPos) = 0;
	//section被选中之后
	virtual void sectionSelected(int sectionId,CCPoint sectionPos) = 0;
	//section页面放弃选择，也就是返回了
	virtual void sectionBackToSeason(int fromSeasonId) = 0;
};


#endif