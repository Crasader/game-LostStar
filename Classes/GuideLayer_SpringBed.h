#ifndef __GuideLayer_SpringBed_H__
#define __GuideLayer_SpringBed_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_SpringBed : public BaseGuideLayer
{
private:
	void baseContentLayerEffctiveClicked(CCNode* node);
	void lfClick(CCPoint glPoint);
	CCSprite* guide_arrows;
	CCLabelTTF* guide_txt;
public:
	GuideLayer_SpringBed(int seasonId,int sectionId);
	void startGuide();
};

#endif