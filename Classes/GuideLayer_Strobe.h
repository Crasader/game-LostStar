#ifndef __GuideLayer_Strobe_H__
#define __GuideLayer_Strobe_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_Strobe : public BaseGuideLayer
{
private:
	void baseContentLayerEffctiveClicked(CCNode* node);
	void lfClick(CCPoint glPoint);
public:
	GuideLayer_Strobe(int seasonId,int sectionId);
};

#endif