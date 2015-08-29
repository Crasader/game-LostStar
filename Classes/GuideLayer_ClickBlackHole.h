#ifndef __GuideLayer_ClickBlackHole_H__
#define __GuideLayer_ClickBlackHole_H__
#include "BaseGuideLayer.h"

/**
 *	这用于
 */
class GuideLayer_ClickBlackHole : public BaseGuideLayer
{
private:
	void lfClick(CCPoint glPoint);

	void baseContentLayerEffctiveClicked(CCNode* node);

	CCSprite* m_pGuide_arrows;
	CCLabelTTF* m_pGuide_txt; 
public:
	GuideLayer_ClickBlackHole(int seasonId,int sectionId);
	void startGuide();
};

#endif