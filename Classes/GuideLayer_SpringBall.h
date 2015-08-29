#ifndef __GuideLayer_SpringBall_H__
#define __GuideLayer_SpringBall_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_SpringBall : public BaseGuideLayer
{
private:
	void baseContentLayerEffctiveClicked(CCNode* node);
	void lfClick(CCPoint glPoint);
	CCSprite* guide_arrows;
	CCLabelTTF* guide_txt;
public:
	GuideLayer_SpringBall(int seasonId,int sectionId);
	void startGuide();
};

#endif