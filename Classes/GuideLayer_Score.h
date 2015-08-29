#ifndef __GuideLayer_Score_H__
#define __GuideLayer_Score_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_Score : public BaseGuideLayer
{
private:
	void baseContentLayerEffctiveClicked(CCNode* node);
	void lfClick(CCPoint glPoint);
	CCLabelTTF* guide_txt;
public:
	GuideLayer_Score(int seasonId,int sectionId);
	void startGuide();
};

#endif