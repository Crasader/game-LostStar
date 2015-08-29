#ifndef __GuideLayer_Bomb_H__
#define __GuideLayer_Bomb_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_Bomb : public BaseGuideLayer
{
private:
	void baseContentLayerEffctiveClicked(CCNode* node);
	void lfClick(CCPoint glPoint);
public:
	GuideLayer_Bomb(int seasonId,int sectionId);
};

#endif