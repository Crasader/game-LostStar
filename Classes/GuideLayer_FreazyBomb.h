#ifndef __GuideLayer_FreazyBomb_H__
#define __GuideLayer_FreazyBomb_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_FreazyBomb : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);
	void baseContentLayerEffctiveClicked(CCNode* node);
public:
	GuideLayer_FreazyBomb(int seasonId,int sectionId);
	void startGuide();
};

#endif