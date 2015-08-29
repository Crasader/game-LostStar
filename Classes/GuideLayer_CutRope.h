#ifndef __GuideLayer_CutRope_H__
#define __GuideLayer_CutRope_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_CutRope : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);
	void baseContentLayerEffctiveClicked(CCNode* node);
public:
	GuideLayer_CutRope(int seasonId,int sectionId);
};

#endif