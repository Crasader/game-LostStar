#ifndef __GuideLayer_DoubleTable_H__
#define __GuideLayer_DoubleTable_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_DoubleTable : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);

	CCSprite* spriteMask1;
	CCSprite* spriteMask2;
	CCLabelTTF* guide_txt;

public:
	GuideLayer_DoubleTable(int seasonId,int sectionId);
	void startGuide();
	void stopGuide();
};

#endif