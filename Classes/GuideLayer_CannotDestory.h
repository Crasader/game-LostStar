#ifndef __GuideLayer_CannotDestory_H__
#define __GuideLayer_CannotDestory_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_CannotDestory : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);

	CCSprite* guide_arrows;
	CCLabelTTF* guide_txt;

public:
	GuideLayer_CannotDestory(int seasonId,int sectionId);
	void startGuide();
};

#endif